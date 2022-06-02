#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

in DATA
{
	vec3 position;
	vec2 uv;
	mat3 normal;
} fs_in;

struct DirectionalLight
{
	vec3 position;
	vec3 direction;
	vec3 color;
};

struct PointLight
{
	vec3 position;
	vec3 color;

	float constant;
	float linear;
	float quadratic;
};

struct Material
{
	sampler2D diffuseMap;
	sampler2D normalMap;
	sampler2D metallicMap;
	sampler2D roughnessMap;
	sampler2D ambientMap;
};

struct Params
{
	vec3 diffuse;
	float metallic;
	float roughness;
	float ambient;

	vec3 reflectCol;

	vec3 normal;
	vec3 view;

	vec3 F0;
};

Params params;

uniform samplerCube u_ShadowDepthMap0;
uniform samplerCube u_ShadowDepthMap1;
uniform samplerCube u_ShadowDepthMap2;
uniform samplerCube u_ShadowDepthMap3;
uniform float u_ShadowDistance;

uniform samplerCube u_SkyboxMap;

uniform Material u_Material;

uniform vec3 u_CameraPosition;

uniform int u_NumPointLights;
uniform int u_NumDirectionalLights;

uniform DirectionalLight u_DirectionalLights[1];
uniform PointLight u_PointLights[3];

const float PI = 3.14159265359;

vec3 gridSamplingDisk[20] = vec3[](
	vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
	vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
	vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

float PointShadowCalculation(samplerCube depthMap, vec3 lightPos)
{
	vec3 lightRelative = fs_in.position - lightPos;
	float fragDepth = length(lightRelative);

	float viewDistance = length(u_CameraPosition - fs_in.position);
	float diskRadius = (1.0 + (viewDistance / u_ShadowDistance)) / 25.0;

	float shadow = 0.0;

	float bias = 0.001;
			
	int samples = 20;
	for (int i = 0; i < samples; ++i)
	{
		float depth = texture(depthMap, lightRelative + gridSamplingDisk[i] * diskRadius).r * u_ShadowDistance;
		if (fragDepth + bias > depth)
			shadow += 1.0;
	}

	return shadow / float(samples);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
};

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
};

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
};

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
};

vec3 ApplyLight(vec3 lightDirection, vec3 lightColor, float lightAttenuation)
{
	// calculate per-light radiance
	vec3 H = normalize(params.view + lightDirection);
	vec3 radiance = lightColor * lightAttenuation;

	// Cook-Torrance BRDF
	float NDF = DistributionGGX(params.normal, H, params.roughness);
	float G = GeometrySmith(params.normal, params.view, lightDirection, params.roughness);
	vec3 F = fresnelSchlick(max(dot(H, params.view), 0.0), params.F0);

	vec3 numerator = F * NDF * G;
	float denominator = 4 * max(dot(params.normal, params.view), 0.0) * max(dot(params.normal, lightDirection), 0.0);
	vec3 specular = (numerator / (denominator + 0.0001));

	// kS is equal to Fresnel
	vec3 kS = F;
	// for energy conservation, the diffuse and specular light can't
	// be above 1.0 (unless the surface emits light); to preserve this
	// relationship the diffuse component (kD) should equal 1.0 - kS.
	vec3 kD = vec3(1.0) - kS;
	// multiply kD by the inverse metalness such that only non-metals
	// have diffuse lighting, or a linear blend if partly metal (pure metals
	// have no diffuse light).
	kD *= 1.0 - params.metallic;

	// scale light by NdotL
	float NdotL = max(dot(params.normal, lightDirection), 0.0);

	// only metallic surfaces can reflect
	vec3 reflection = (1.0 - kD) * params.reflectCol;

	// add to outgoing radiance Lo
	return ((kD * params.diffuse / PI) + specular + reflection) * radiance * NdotL;
};

vec3 ApplyPointLight(PointLight light)
{
	vec3 lightDirection = normalize(light.position - fs_in.position);
	float distance = length(light.position - fs_in.position);
	float lightAttenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	return ApplyLight(lightDirection, light.color, lightAttenuation);
};

void main()
{
	params.diffuse = pow(texture(u_Material.diffuseMap, fs_in.uv).rgb, vec3(2.2));
	params.metallic = texture(u_Material.metallicMap, fs_in.uv).r;
	params.roughness = texture(u_Material.roughnessMap, fs_in.uv).r;
	params.ambient = texture(u_Material.ambientMap, fs_in.uv).r;

	vec3 normalMap = texture(u_Material.normalMap, fs_in.uv).xyz;
	params.normal = normalize((normalMap * 2.0 - 1.0));
	params.view = normalize(u_CameraPosition - fs_in.position);

	params.F0 = mix(vec3(0.04), params.diffuse, params.metallic);

	params.reflectCol = vec3(1.0, 1.0, 1.0);//texture(u_SkyboxMap, reflect(-params.view, params.normal)).rgb;

	float shadows[4];
	
	shadows[0] = 1.0;// - PointShadowCalculation(u_ShadowDepthMap0, u_DirectionalLights[0].position);
	shadows[1] = 1.0;// - PointShadowCalculation(u_ShadowDepthMap1, u_PointLights[0].position);
	shadows[2] = 1.0;// - PointShadowCalculation(u_ShadowDepthMap2, u_PointLights[1].position);
	shadows[3] = 1.0;// - PointShadowCalculation(u_ShadowDepthMap3, u_PointLights[2].position);

	vec3 color = vec3(0.5) * params.diffuse * params.ambient;

	for (int i = 0; i < u_NumDirectionalLights; i++)
	{
		color += shadows[i] * ApplyLight(u_DirectionalLights[i].direction, u_DirectionalLights[i].color, 1.0);
	}

	for (int i = 0; i < u_NumPointLights; i++)
	{
		color += shadows[u_NumDirectionalLights + i] * ApplyPointLight(u_PointLights[i]);
	}

	float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0)
		BrightColor = vec4(color, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

	FragColor = vec4(color, 1.0);
};
