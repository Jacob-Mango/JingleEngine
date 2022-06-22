#include "Material.h"

#include "Config/Config.h"

#include <vector>
#include <string>
#include <fstream>

BEGIN_CLASS_LINK(MaterialImage)
	LINK_NAMED_VARIABLE(File, m_File);
	LINK_NAMED_VARIABLE(Name, m_Name);
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

BEGIN_STRUCTURE_LINK(MaterialImageArray)
	LINK_CONSTRUCTOR();
	LINK_METHOD(Insert);
END_STRUCTURE_LINK()

void MaterialImageArray::Insert(JingleScript::Object* value)
{
	push_back(static_cast<MaterialImage*>(value));
}

BEGIN_CLASS_LINK(Material)
	LINK_NAMED_VARIABLE(Shader, m_Shader);
	LINK_NAMED_VARIABLE(Images, m_Images);
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

Material::Material()
{
}

Material::~Material()
{
}

bool Material::OnLoad()
{
	if (!Super::OnLoad())
	{
		return false;
	}

	if (!WriteToObject(this))
	{
		return false;
	}

	return true;
}

void Material::Bind(Ref<Shader> shader)
{
	int index = 0;
	for (auto& image : m_Images)
	{
		auto& file = image->m_File;
		auto& name = image->m_Name;

		std::string uniform = "u_Material." + name;

		m_Shader->Set(uniform, index);
		file->Bind(index);

		index++;
	}
}

void Material::Unbind(Ref<Shader> shader)
{
	for (auto& image : m_Images)
	{
		auto& file = image->m_File;

		file->Unbind();
	}
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
