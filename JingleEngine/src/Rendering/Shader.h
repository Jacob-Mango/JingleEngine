#pragma once

#include "Core/Core.h"

#include "Asset/Asset.h"

#ifdef JE_EDITOR
#include "Editor/Editors/Shader/ShaderGraph.h"
#endif

class Shader : public Asset
{
	SETUP_ASSET(Shader, Asset)

	GLuint m_ID;
	GLuint m_VertexShaderID;
	GLuint m_FragmentShaderID;
	GLuint m_GeometryShaderID;

#ifdef JE_EDITOR
	ShaderGraph* m_Graph = nullptr;
	Config* m_Config = nullptr;
#endif

public:
	Shader();
	virtual ~Shader();

	virtual bool OnLoad() override;
	virtual bool OnSave() override;

	void Bind();
	void Unbind();

	unsigned int GetLocation(std::string name);

	void Set(unsigned int location, float value);
	void Set(unsigned int location, float* value, unsigned int count);
	void Set(unsigned int location, int value);
	void Set(unsigned int location, int* value, unsigned int count);
	void Set(unsigned int location, const glm::vec2& value, unsigned int count = 1);
	void Set(unsigned int location, const glm::vec3& value, unsigned int count = 1);
	void Set(unsigned int location, const glm::vec4& value, unsigned int count = 1);
	void Set(unsigned int location, const glm::mat3& value, unsigned int count = 1);
	void Set(unsigned int location, const glm::mat4& value, unsigned int count = 1);

	void Set(std::string name, float value);
	void Set(std::string name, float* value, unsigned int count);
	void Set(std::string name, int value);
	void Set(std::string name, int* value, unsigned int count);
	void Set(std::string name, const glm::vec2& value, unsigned int count = 1);
	void Set(std::string name, const glm::vec3& value, unsigned int count = 1);
	void Set(std::string name, const glm::vec4& value, unsigned int count = 1);
	void Set(std::string name, const glm::mat3& value, unsigned int count = 1);
	void Set(std::string name, const glm::mat4& value, unsigned int count = 1);

private:
	int LoadShader(std::string path, GLuint type, GLuint& id);

#ifdef JE_EDITOR
public:
	ShaderGraph* GetGraph() const;

private:
	bool LoadGraph(const std::string& path);
	bool SaveGraph(const std::string& path);
#endif

};
