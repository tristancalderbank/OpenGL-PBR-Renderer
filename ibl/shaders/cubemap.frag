#version 330 core

out vec4 FragColor;
in vec3 modelCoordinates;

uniform samplerCube cubemap;

void main() {
	vec3 sampleDirection = normalize(modelCoordinates);
	vec3 color = texture(cubemap, sampleDirection).rgb;
	FragColor = vec4(color, 1.0);
}