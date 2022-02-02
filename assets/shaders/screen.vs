#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_UV;

out DATA
{
	vec2 uv;
} vs_out;

void main()
{
    vs_out.uv = in_UV;
    gl_Position = vec4(in_Position, 1.0);
};
