#pragma once

#include "Core/Core.h"

#include "Asset/Asset.h"
#include "Asset/AssetModule.h"

#include "Config/ConfigAsset.h"

#include "Rendering/Texture.h"
#include "Rendering/Shader.h"

class Material;

class MaterialImage : public JingleScript::Object
{
	DEFINE_CLASS(MaterialImage, JingleScript::Object);

	friend Material;

private:
	Image* m_File = nullptr;
	std::string m_Name;

public:
	MaterialImage() {}
	~MaterialImage() {}

	virtual std::string ToString() const override
	{
		return fmt::format("{}: {}", m_Name, m_File ? m_File->GetPath() : "no file");
	}

};

class MaterialImageArray : public JingleScript::Array<MaterialImage*>
{
	DEFINE_CLASS(MaterialImageArray, JingleScript::Array<MaterialImage*>);

public:
	MaterialImageArray() {}
	~MaterialImageArray() {}

};

class Material : public ConfigAsset
{
	SETUP_ASSET(Material, ConfigAsset)

public: //! temp vis
	Shader* m_Shader = nullptr;
	MaterialImageArray* m_Images;

public:
	Material();
	virtual ~Material();

	virtual bool OnLoad() override;

	void Bind(Ref<Shader> shader);
	void Unbind(Ref<Shader> shader);

	Ref<Shader> GetShader();

	void Set(unsigned int location, float value);
	void Set(unsigned int location, float* value, unsigned int count);
	void Set(unsigned int location, int value);
	void Set(unsigned int location, int* value, unsigned int count);
	void Set(unsigned int location, const glm::vec2& value, unsigned int count = 1);
	void Set(unsigned int location, const glm::vec3& value, unsigned int count = 1);
	void Set(unsigned int location, const glm::vec4& value, unsigned int count = 1);
	void Set(unsigned int location, const glm::mat4& value, unsigned int count = 1);

	void Set(std::string name, float value);
	void Set(std::string name, float* value, unsigned int count);
	void Set(std::string name, int value);
	void Set(std::string name, int* value, unsigned int count);
	void Set(std::string name, const glm::vec2& value, unsigned int count = 1);
	void Set(std::string name, const glm::vec3& value, unsigned int count = 1);
	void Set(std::string name, const glm::vec4& value, unsigned int count = 1);
	void Set(std::string name, const glm::mat4& value, unsigned int count = 1);
};
