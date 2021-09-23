#version 330 core

out vec4 FragColor;
in vec3 textureCoordinates;

uniform samplerCube skybox;

void main() {
	FragColor = texture(skybox, textureCoordinates); // for samplerCube the coordinates are a vector
}