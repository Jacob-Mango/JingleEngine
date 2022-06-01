#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec3 in_Tangent;
layout (location = 3) in vec2 in_UV;

/*
layout (std140) uniform Camera
{
	uniform mat4 u_ProjectionMatrix;
	uniform mat4 u_ViewMatrix;
};

layout (std140) uniform Model
{
	uniform mat4 u_ModelMatrix;
};
*/

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;

out DATA
{
	vec3 position;
	vec3 normal;
	vec2 uv;
} vs_out;

void main()
{
	vs_out.position = vec3(u_ModelMatrix * vec4(in_Position, 1.0));
	vs_out.normal = mat3(transpose(inverse(u_ModelMatrix))) * in_Normal;
	vs_out.uv = in_UV;

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(vs_out.position, 1.0);
};