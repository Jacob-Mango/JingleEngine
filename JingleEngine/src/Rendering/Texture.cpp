#include "Texture.h"

#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
}

GLenum Texture::FormatToGL(TextureFormat format)
{
	switch (format)
	{
	case TextureFormat::RGBA8:
		return GL_RGB8;
	case TextureFormat::RGBA16:
		return GL_RGBA16F;
	case TextureFormat::RGBA32:
		return GL_RGBA32F;
	case TextureFormat::DEPTH:
		return GL_DEPTH_COMPONENT32F;
	}
	return 0;
}

void Texture::Bind()
{
	glBindTexture(m_CubeMap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, m_ID);
}

void Texture::Bind(int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(m_CubeMap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, m_ID);
}

void Texture::Unbind()
{
	glBindTexture(m_CubeMap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, 0);
}

void Texture::Create(TextureFormat format, unsigned int width, unsigned int height, bool cubeMap)
{
	m_Format = format;
	m_Width = width;
	m_Height = height;
	m_CubeMap = cubeMap;

	glGenTextures(1, &m_ID);

	Bind();

	if (m_CubeMap)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, FormatToGL(m_Format), m_Width, m_Height);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexStorage2D(GL_TEXTURE_2D, 1, FormatToGL(m_Format), m_Width, m_Height);
	}

	Unbind();
}

bool Texture::OnLoad()
{
	if (GetPath() == "")
		return false;

	std::string path = GetPath();
	std::string extension = GetExtension();

	if (extension.compare("cubemap") == 0)
	{
		m_CubeMap = true;

		path = path.substr(0, path.size() - 7);

		unsigned char* data[6];
		int components;

		stbi_set_flip_vertically_on_load(true);
		for (int i = 0; i < 6; i++)
			data[i] = stbi_load((path + std::to_string(i) + ".png").c_str(), (int*)&m_Width, (int*)&m_Height, &components, STBI_rgb);
		m_Format = TextureFormat::RGBA16;

		glGenTextures(1, &m_ID);
		Bind();

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (int i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, FormatToGL(m_Format), m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data[i]);

		Unbind();

		for (int i = 0; i < 6; i++)
			stbi_image_free(data[i]);

		return true;
	}

	if (extension.compare("png") == 0)
	{
		m_CubeMap = false;

		int components;
		unsigned char* data;

		stbi_set_flip_vertically_on_load(true);
		data = stbi_load(path.c_str(), (int*)&m_Width, (int*)&m_Height, &components, STBI_rgb);
		m_Format = TextureFormat::RGBA16;

		glGenTextures(1, &m_ID);
		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, FormatToGL(m_Format), m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		Unbind();
		stbi_image_free(data);

		return true;
	}

	return false;
}
