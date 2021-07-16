#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoordinates;

out vec2 textureCoordinates;

void main() {
	gl_Position = vec4(aPos, 1.0f); // input coordinates are already in NDC
	textureCoordinates = aTextureCoordinates;
};