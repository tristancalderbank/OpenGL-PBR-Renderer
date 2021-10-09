#version 330 core

out vec4 FragColor;
in vec2 textureCoordinates;

uniform sampler2D colorTexture;

void main() {
	vec3 color = texture(colorTexture, textureCoordinates).rgb;

	color = color / (color + vec3(1.0)); // HDR, apply Reinhard tonemapping C = C / (1 + C)
	color = pow(color, vec3(1.0 / 2.2)); // gamma correction to account for monitor, raise to the (1 / 2.2)

	FragColor = vec4(color, 1.0);
}