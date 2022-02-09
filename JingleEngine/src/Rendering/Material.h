#pragma once

#include "Core/Core.h"
#include "Core/Reference.h"

#include "Asset/Asset.h"
#include "Asset/AssetModule.h"

#include "Rendering/Texture.h"
#include "Rendering/Shader.h"

class Material : public Asset
{
	SETUP_ASSET(Material, Asset)

	Ref<Shader> m_Shader;

	//TODO: move to an array/map and assign based on uniforms found within the shader
	Ref<Texture> m_Ambient;
	Ref<Texture> m_Diffuse;
	Ref<Texture> m_Metallic;
	Ref<Texture> m_Normal;
	Ref<Texture> m_Roughness;

public:
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
