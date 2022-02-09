#include "Material.h"

#include "Core/Config.h"

#include <vector>
#include <string>
#include <fstream>

Material::~Material()
{
}

bool Material::OnLoad()
{
	if (GetPath() == "") return false;

	auto config = Config::Load(GetPath());

	std::cout << "MATERIAL:: " << config->AsString() << std::endl;

	std::string shader = (*config)["shader"].String;
	m_Shader = AssetManager::Get<Shader>(shader);

	std::string ambient = (*config)["ambient"]["path"].String;
	m_Ambient = AssetManager::Get<Texture>(ambient);

	std::string diffuse = (*config)["diffuse"]["path"].String;
	m_Diffuse = AssetManager::Get<Texture>(diffuse);

	std::string metallic = (*config)["metallic"]["path"].String;
	m_Metallic = AssetManager::Get<Texture>(metallic);

	std::string normal = (*config)["normal"]["path"].String;
	m_Normal = AssetManager::Get<Texture>(normal);

	std::string roughness = (*config)["roughness"]["path"].String;
	m_Roughness = AssetManager::Get<Texture>(roughness);


	#if 0
	{
		auto shader = (*config)["shader"];
		auto path = shader.String;

		m_Shader = AssetManager::Get<Shader>(path);
	}

	{
		auto ambient = (*config)["ambient"];
		auto path = ambient["path"].String;

		m_Ambient = AssetManager::Get<Texture>(path);
	}

	{
		auto diffuse = (*config)["diffuse"];
		auto path = diffuse["path"].String;

		m_Diffuse = AssetManager::Get<Texture>(path);
	}

	{
		auto metallic = (*config)["metallic"];
		auto path = metallic["path"].String;

		m_Metallic = AssetManager::Get<Texture>(path);
	}

	{
		auto normal = (*config)["normal"];
		auto path = normal["path"].String;

		m_Normal = AssetManager::Get<Texture>(path);
	}

	{
		auto roughness = (*config)["roughness"];
		auto path = roughness["path"].String;

		m_Roughness = AssetManager::Get<Texture>(path);
	}
	#endif

	std::cout << "Shader: " << m_Shader.AsString() << std::endl;
	std::cout << "Ambient: " << m_Ambient.AsString() << std::endl;
	std::cout << "Diffuse: " << m_Diffuse.AsString() << std::endl;
	std::cout << "Metallic: " << m_Metallic.AsString() << std::endl;
	std::cout << "Normal: " << m_Normal.AsString() << std::endl;
	std::cout << "Roughness: " << m_Roughness.AsString() << std::endl;

	return true;
}

void Material::Bind(Ref<Shader> shader)
{
	m_Shader->Set("u_Material.ambientMap", 0);
	m_Ambient->Bind(0);

	m_Shader->Set("u_Material.diffuseMap", 1);
	m_Diffuse->Bind(1);

	m_Shader->Set("u_Material.metallicMap", 2);
	m_Metallic->Bind(2);

	m_Shader->Set("u_Material.normalMap", 3);
	m_Normal->Bind(3);

	m_Shader->Set("u_Material.roughnessMap", 4);
	m_Roughness->Bind(4);
}

void Material::Unbind(Ref<Shader> shader)
{
	m_Ambient->Unbind();
	m_Diffuse->Unbind();
	m_Metallic->Unbind();
	m_Normal->Unbind();
	m_Roughness->Unbind();
}

Ref<Shader> Material::GetShader()
{
	return m_Shader;
}

void Material::Set(unsigned int location, float value)
{
	m_Shader->Set(location, value);
}

void Material::Set(unsigned int location, float* value, unsigned int count)
{
	m_Shader->Set(location, value, count);
}

void Material::Set(unsigned int location, int value)
{
	m_Shader->Set(location, value);
}

void Material::Set(unsigned int location, int* value, unsigned int count)
{
	m_Shader->Set(location, value, count);
}

void Material::Set(unsigned int location, const glm::vec2& value, unsigned int count)
{
	m_Shader->Set(location, value, count);
}

void Material::Set(unsigned int location, const glm::vec3& value, unsigned int count)
{
	m_Shader->Set(location, value, count);
}

void Material::Set(unsigned int location, const glm::vec4& value, unsigned int count)
{
	m_Shader->Set(location, value, count);
}

void Material::Set(unsigned int location, const glm::mat4& value, unsigned int count)
{
	m_Shader->Set(location, value, count);
}

void Material::Set(std::string name, float value)
{
	m_Shader->Set(name, value);
}

void Material::Set(std::string name, float* value, unsigned int count)
{
	m_Shader->Set(name, value, count);
}

void Material::Set(std::string name, int value)
{
	m_Shader->Set(name, value);
}

void Material::Set(std::string name, int* value, unsigned int count)
{
	m_Shader->Set(name, value, count);
}

void Material::Set(std::string name, const glm::vec2& value, unsigned int count)
{
	m_Shader->Set(name, value, count);
}

void Material::Set(std::string name, const glm::vec3& value, unsigned int count)
{
	m_Shader->Set(name, value, count);
}

void Material::Set(std::string name, const glm::vec4& value, unsigned int count)
{
	m_Shader->Set(name, value, count);
}

void Material::Set(std::string name, const glm::mat4& value, unsigned int count)
{
	m_Shader->Set(name, value, count);
}
