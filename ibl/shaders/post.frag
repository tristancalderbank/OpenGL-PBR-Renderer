#version 330 core

out vec4 FragColor;
in vec2 textureCoordinates;

uniform sampler2D colorTexture;

void main() {
	vec3 color = texture(colorTexture, textureCoordinates).rgb;
	
	color = color / (color + vec3(1.0)); // HDR, apply Reinhard tonemapping C = C / (1 + C)

	FragColor = vec4(color, 1.0);
}