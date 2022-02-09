#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec3 in_Tangent;
layout(location = 3) in vec2 in_UV;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;

out DATA
{
	vec3 position;
	vec2 uv;
	mat3 normal;
} vs_out;

void main()
{
	vs_out.position = vec3(u_ModelMatrix * vec4(in_Position, 1.0));
	vs_out.uv = in_UV;

	vec3 T = normalize(vec3(u_ModelMatrix * vec4(in_Tangent, 0.0)));
	vec3 N = normalize(vec3(u_ModelMatrix * vec4(in_Normal, 0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	
	vs_out.normal = mat3(T, B, N);

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(vs_out.position, 1.0);
};
