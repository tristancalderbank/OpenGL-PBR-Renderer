#version 330 core

out vec4 FragColor;
in vec3 normal;
in vec2 textureCoordinates;

struct Material {
  sampler2D textureDiffuse0;
};

uniform Material material;

void main() {
	FragColor = texture(material.textureDiffuse0, textureCoordinates);
};