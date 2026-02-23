#version 450 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D simTex;

void main() {
	float d = texture(simTex, TexCoords).r;

	float intensity = pow(d, 0.8) * 40.0;

	vec3 color = vec3(1.0, 0.1, 0.1) * intensity;

	FragColor = vec4(clamp(color, 0.0, 1.0), 1.0);
}