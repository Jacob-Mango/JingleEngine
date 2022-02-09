#version 330 core

out vec4 FragColor;

in DATA
{
	vec3 position;
	vec3 normal;
	vec2 uv;
} fs_in;

void main()
{
	float terrainSize = 128.0;

	float ambientStrength = 0.1;
	float diffuseStrength = 1.0;
	vec3 lightPos = vec3(terrainSize * 0.5);
	vec3 objectColor = vec3(1.0, 0.5, 0.31);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	vec3 ambient = ambientStrength * lightColor;

	vec3 lightDir = normalize(lightPos - fs_in.position);
	float diff = max(dot(fs_in.normal, lightDir), 0.0) * diffuseStrength;
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse) * objectColor;

	FragColor = vec4(result, 1.0);
}