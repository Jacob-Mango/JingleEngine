#include "Shader.h"

#include <fstream>
#include <iostream>
#include <filesystem>

Shader::Shader()
{
}

Shader::~Shader()
{
	GL(glUseProgram(0));

	if (m_VertexShaderID)
		GL(glDetachShader(m_ID, m_VertexShaderID));
	if (m_FragmentShaderID)
		GL(glDetachShader(m_ID, m_FragmentShaderID));
	if (m_GeometryShaderID)
		GL(glDetachShader(m_ID, m_GeometryShaderID));

	if (m_VertexShaderID)
		GL(glDeleteShader(m_FragmentShaderID));
	if (m_FragmentShaderID)
		GL(glDeleteShader(m_VertexShaderID));
	if (m_GeometryShaderID)
		GL(glDeleteShader(m_GeometryShaderID));

	GL(glDeleteProgram(m_ID));
}

BEGIN_CLASS_LINK(Shader)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

bool Shader::OnLoad()
{
	JS_TRACE(Tracers::Rendering);

	if (GetPath() == "")
		return false;

	std::string path = GetPath();

	int vertex = LoadShader(path + ".vs", GL_VERTEX_SHADER, m_VertexShaderID);
	int fragment = LoadShader(path + ".fs", GL_FRAGMENT_SHADER, m_FragmentShaderID);
	int geometry = LoadShader(path + ".gs", GL_GEOMETRY_SHADER, m_GeometryShaderID);

	GL(m_ID = glCreateProgram());

	if (vertex == 0 || fragment == 0 || geometry == 0)
	{
		return false;
	}
	else
	{
		if (vertex == 1)
			GL(glAttachShader(m_ID, m_VertexShaderID));

		if (fragment == 1)
			GL(glAttachShader(m_ID, m_FragmentShaderID));

		if (geometry == 1)
			GL(glAttachShader(m_ID, m_GeometryShaderID));
	}

	GL(glLinkProgram(m_ID));

	GLint status;
	GL(glGetProgramiv(m_ID, GL_LINK_STATUS, &status));
	if (status == GL_FALSE)
	{
		GLchar infoLog[1024];
		GL(glGetProgramInfoLog(m_ID, 1024, NULL, infoLog));
		JS_ERROR("Failed to link shaders ({}): {}", path, infoLog);
		return false;
	}

	GL(glUseProgram(0));

	return true;
}

int Shader::LoadShader(std::string path, GLuint type, GLuint& id)
{
	JS_TRACE(Tracers::Rendering);

	if (strcmp(path.c_str(), "") == 0)
		return 2;

	if (!std::filesystem::exists(path))
		return 2;

	std::ifstream file(path);
	if (file.eof())
		return 2;

	JS_TINFO("Loading {} shader: {}", type, path);

	std::string source;

	std::string line;
	while (std::getline(file, line))
		source += line + "\n";

	GL(id = glCreateShader(type));

	const char* sourceCstr = source.c_str();
	GL(glShaderSource(id, 1, &sourceCstr, NULL));
	GL(glCompileShader(id));

	GLint status;
	GL(glGetShaderiv(id, GL_COMPILE_STATUS, &status));
	if (status == GL_FALSE)
	{
		GLchar infoLog[1024];
		GL(glGetShaderInfoLog(id, 1024, NULL, infoLog));
		JS_ERROR("Failed to compile shader ({}): {}", path, infoLog);
		return 0;
	}

	JS_TINFO("Loaded.");
	return 1;
}

void Shader::Bind()
{
	GL(glUseProgram(m_ID));
}

void Shader::Unbind()
{
	GL(glUseProgram(0));
}

unsigned int Shader::GetLocation(std::string name)
{
	GL(unsigned int location = glGetUniformLocation(m_ID, name.c_str()));
	return (unsigned int)location;
}

void Shader::Set(unsigned int location, float value)
{
	GL(glUniform1f(location, value));
}

void Shader::Set(unsigned int location, float* value, unsigned int count)
{
	GL(glUniform1fv(location, count, value));
}

void Shader::Set(unsigned int location, int value)
{
	GL(glUniform1i(location, value));
}

void Shader::Set(unsigned int location, int* value, unsigned int count)
{
	GL(glUniform1iv(location, count, value));
}

void Shader::Set(unsigned int location, const glm::vec2& value, unsigned int count)
{
	GL(glUniform2fv(location, count, glm::value_ptr(value)));
}

void Shader::Set(unsigned int location, const glm::vec3& value, unsigned int count)
{
	GL(glUniform3fv(location, count, glm::value_ptr(value)));
}

void Shader::Set(unsigned int location, const glm::vec4& value, unsigned int count)
{
	GL(glUniform4fv(location, count, glm::value_ptr(value)));
}

void Shader::Set(unsigned int location, const glm::mat3& value, unsigned int count)
{
	GL(glUniformMatrix3fv(location, count, GL_FALSE, &value[0][0]));
}

void Shader::Set(unsigned int location, const glm::mat4& value, unsigned int count)
{
	GL(glUniformMatrix4fv(location, count, GL_FALSE, &value[0][0]));
}

void Shader::Set(std::string name, float value)
{
	Set(GetLocation(name), value);
}

void Shader::Set(std::string name, float* value, unsigned int count)
{
	Set(GetLocation(name), value, count);
}

void Shader::Set(std::string name, int value)
{
	Set(GetLocation(name), value);
}

void Shader::Set(std::string name, int* value, unsigned int count)
{
	Set(GetLocation(name), value, count);
}

void Shader::Set(std::string name, const glm::vec2& value, unsigned int count)
{
	Set(GetLocation(name), value, count);
}

void Shader::Set(std::string name, const glm::vec3& value, unsigned int count)
{
	Set(GetLocation(name), value, count);
}

void Shader::Set(std::string name, const glm::vec4& value, unsigned int count)
{
	Set(GetLocation(name), value, count);
}

void Shader::Set(std::string name, const glm::mat3& value, unsigned int count)
{
	Set(GetLocation(name), value, count);
}

void Shader::Set(std::string name, const glm::mat4& value, unsigned int count)
{
	Set(GetLocation(name), value, count);
}
