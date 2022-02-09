#version 330 core

layout (location = 0) out vec4 FragColor;

in DATA
{
	vec2 uv;
} fs_in;

uniform sampler2D u_Screen;

void main()
{
	FragColor = vec4(texture(u_Screen, fs_in.uv).rgb, 1.0);
};
