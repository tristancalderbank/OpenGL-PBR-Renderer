#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoordinates;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 normal;
out vec2 textureCoordinates;
out vec3 worldCoordinates;
out mat3 TBN; // tangent to world matrix

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	mat4 modelView = view * model;
	worldCoordinates = (model * vec4(aPos, 1.0f)).xyz;
	gl_Position = projection * modelView * vec4(aPos, 1.0f);
	textureCoordinates = aTextureCoordinates;

	mat3 normalMatrix = mat3(transpose(inverse(modelView)));

	vec3 T = normalize(normalMatrix * aTangent);
	vec3 B = normalize(normalMatrix * aBitangent);
	vec3 N = normalize(normalMatrix * aNormal);
	normal = aNormal;
	TBN = mat3(T, B, N);
}