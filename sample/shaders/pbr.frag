#version 330 core

#define PI 3.1415926535897932384626433832795
#define GAMMA 2.2 // TODO: make this a uniform and adjustable from imgui

out vec4 FragColor;
in vec3 worldCoordinates;
in vec3 normal;
in vec2 textureCoordinates;

struct Material {
  sampler2D albedo;
  sampler2D metallicRoughness;
  sampler2D normal;
  sampler2D ambientOcclusion;
  sampler2D emissive;
};

uniform Material material;

uniform vec3 cameraPosition;

// lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

// PBR
uniform float ambientOcclusion;

// IBL precomputed maps
const float PREFILTERED_ENV_MAP_LOD = 4.0; // how many mipmap levels

uniform samplerCube diffuseIrradianceMap;
uniform samplerCube prefilteredEnvMap;
uniform sampler2D brdfConvolutionMap;

// Fresnel function (Fresnel-Schlick approximation)
//
// F_schlick = f0 + (1 - f0)(1 - (h * v))^5
//
vec3 fresnelSchlick(float cosTheta, vec3 f0) { 
	return f0 + (1.0 - f0) * pow(max(1 - cosTheta, 0.0), 5.0);
}

// Fresnel schlick roughness
//
// Same as above except with a roughness term
vec3 fresnelSchlickRoughness(float cosTheta, vec3 f0, float roughness)
{
	return f0 + (max(vec3(1.0 - roughness), f0) - f0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// Normal distribution function (Trowbridge-Reitz GGX)
//
//                alpha ^ 2
//     ---------------------------------
//      PI((n * h)^2(alpha^2 - 1) + 1)^2
//
float ndfTrowbridgeReitzGGX(vec3 n, vec3 h, float roughness) {
	
	float alpha = roughness * roughness; // recommended by disney/epic papers
	float alphaSquared = alpha * alpha;

	float nDotH = max(dot(n, h), 0.0);
	float nDotHSquared = nDotH * nDotH;
	float innerTerms = nDotHSquared * (alphaSquared - 1.0) + 1.0;

	float numerator = alphaSquared;
	float denomenator = PI * innerTerms * innerTerms;

	return numerator / denomenator;
}

// Geometry function
//
//         n * v
//   -------------------
//   (n * v)(1 - k) + k
//
float geometrySchlickGGX(vec3 n, vec3 v, float k) {

	float nDotV = max(dot(n, v), 0.0);

	float numerator = nDotV;
	float denomenator = nDotV * (1.0 - k) + k;

	return numerator / denomenator;
}

// smiths method for taking into account view direction and light direction
float geometrySmith(vec3 n, vec3 v, vec3 l, float roughness) {

    // remapping for direct lighting (doesn't work for IBL)
	float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;

	return geometrySchlickGGX(n, v, k) * geometrySchlickGGX(n, l, k);
}

void main() {
	// get all the texture values
	vec3 albedo = texture(material.albedo, textureCoordinates).rgb;
	vec2 metallicRoughness = texture(material.metallicRoughness, textureCoordinates).rg;
	float metallic = metallicRoughness.r;
	float roughness = metallicRoughness.g;
	float ao = texture(material.ambientOcclusion, textureCoordinates).r;
	vec3 emissive = texture(material.emissive, textureCoordinates).rgb;

	vec3 n = normalize(normal); // normal
	vec3 v = normalize(cameraPosition - worldCoordinates); // view vector pointing at camera
	vec3 r = reflect(v, n); // reflection

	// f0 is the "surface reflection at zero incidence"
	// for PBR-metallic we assume dialectrics all have 0.04
	// for metals the value comes from the albedo map
	vec3 f0 = vec3(0.04);
	f0 = mix(f0, albedo, metallic);

	vec3 Lo = vec3(0.0); // total radiance out

	// Direct lighting
	// Sum up the radiance contributions of each light source.
	// This loop is essentially the integral of the rendering equation.
	for (int i = 0; i < 4; i++) {
		vec3 l = normalize(lightPositions[i] - worldCoordinates); // light vector
		vec3 h = normalize(v + l);

		float distance = length(lightPositions[i] - worldCoordinates);
		float attenuation = 1.0 / (distance * distance); // inverse square law
		vec3 radiance = lightColors[i] * attenuation; // aka Li

		// calculate Cook-Torrance specular BRDF term
		//
		//                DFG
		//        --------------------
		//         4(w_0 * n)(w_i * n)
		//
		//

		// Normal Distribution term (D)
		float dTerm = ndfTrowbridgeReitzGGX(n, h, roughness);

		// Fresnel term (F)
		// Determines the ratio of light reflected vs. absorbed
		vec3 fTerm = fresnelSchlick(max(dot(h, v), 0.0), f0);

		// Geometry term (G)
		float gTerm = geometrySmith(n, v, l, roughness);


		vec3 numerator = dTerm * fTerm * gTerm;
		float denominator = 4.0 * max(dot(v, n), 0.0) * max(dot(l, n), 0.0);
		
		// recall fTerm is the proportion of reflected light, so the result here is the specular
		vec3 specular = numerator / max(denominator, 0.001);

		vec3 kSpecular = fTerm;
		vec3 kDiffuse = vec3(1.0) - kSpecular;
		kDiffuse *= 1.0 - metallic; // metallic materials should have no diffuse component

		// now calculate full Cook-Torrance with both diffuse + specular
		//
		// f_r = kd * f_lambert + ks * f_cook-torrance
		//
		// where f_lambert = c / pi

		vec3 diffuse = kDiffuse * albedo / PI;
		vec3 cookTorranceBrdf = diffuse + specular;
		float nDotL = max(dot(n, l), 0.0);

		// Finally, the rendering equation!
		Lo += cookTorranceBrdf * radiance * nDotL;
	}

	// Indirect lighting (IBL)
	vec3 kSpecular = fresnelSchlickRoughness(max(dot(n, v), 0.0), f0, roughness); // aka F
    vec3 kDiffuse = 1.0 - kSpecular;
	kDiffuse *= 1.0 - metallic; // metallic materials should have no diffuse component

	// diffuse
    vec3 irradiance = texture(diffuseIrradianceMap, n).rgb;
    vec3 diffuse = irradiance * albedo;

	// specular
	vec3 prefilteredEnvMapColor = textureLod(prefilteredEnvMap, r, roughness * PREFILTERED_ENV_MAP_LOD).rgb;
	float NdotV = max(dot(n, v), 0.0);
	vec2 brdf = texture(brdfConvolutionMap, vec2(NdotV, roughness)).rg;
	vec3 specular = prefilteredEnvMapColor * (kSpecular * brdf.x + brdf.y);

	vec3 ambient = (kDiffuse * diffuse + specular) * ao; // indirect lighting

	// Combine emissive + indirect + direct
	vec3 color = emissive + ambient + Lo;

	FragColor = vec4(color, 1.0);
}