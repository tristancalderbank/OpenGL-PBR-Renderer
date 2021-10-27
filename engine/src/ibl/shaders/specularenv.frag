#version 330 core

out vec4 FragColor;
in vec3 modelCoordinates;

uniform float roughness;
uniform samplerCube environmentCubemap;

const float PI = 3.14159265359;
const uint SAMPLE_COUNT = 1024u;

// this mirrors the number in binary around the decimal point
// aka return: a0 / 2 + a1 / 4 + a2 / 8 + ...
//
// source: http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html#sec-SourceCode
float radicalInverseVanDerCorput(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10;
}

// Hammersley Sequence, which is based on Van der Corput sequence
// This is a "low-discrepancy" sequence, aka quasirandom sequence
// It returns sequence sample i given N total samples
// It corresponds to a point on a unit square [0, 1)
//
// x value is evenly distributed across the unit square
// y is a random value generated with van der corput sequence
vec2 hammersley(uint i, uint N)
{
	return vec2(float(i) / float(N), radicalInverseVanDerCorput(i));
}

vec3 importanceSampleGGX(vec2 unitSquareSample, vec3 N, float roughness) {
	float alpha = roughness * roughness;

	// map the x/y of our unit square sample onto hemisphere
	// using spherical coordinates
	float phi = 2.0 * PI * unitSquareSample.x;
	float cosTheta = sqrt((1.0 - unitSquareSample.y) / (1.0 + (alpha * alpha - 1.0) * unitSquareSample.y));
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

	// spherical to cartesian
	vec3 H; // halfway vector
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;

	// tangent space to world
	vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent   = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);

	vec3 sampleVector = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sampleVector);
}

void main() {
	vec3 N = normalize(modelCoordinates);
	// epic games approximation, view direction is aligned with normal
	vec3 V = N; // view direction

	float totalWeight = 0.0;
	vec3 outputColor = vec3(0.0);

	for(uint i = 0u; i < SAMPLE_COUNT; i++) {
		vec2 unitSquareSample = hammersley(i, SAMPLE_COUNT);
		vec3 H = importanceSampleGGX(unitSquareSample, N, roughness); // halfway
		vec3 L = normalize(2.0 * dot(V, H) * H - V); // light sample direction

		float NdotL = max(dot(N, L), 0.0); // don't forget from the integral
		if(NdotL > 0.0) {
			outputColor += texture(environmentCubemap, L).rgb * NdotL;
			totalWeight += NdotL;
		}
	}
	outputColor /= totalWeight;
	FragColor = vec4(outputColor, 1.0);
}