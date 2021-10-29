#version 330 core

out vec4 FragColor;
in vec3 textureCoordinates;

uniform samplerCube skybox;

void main() {
	FragColor = textureLod(skybox, textureCoordinates, 1.0); // for samplerCube the coordinates are a vector
}