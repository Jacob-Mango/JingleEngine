#include "Rendering/Texture.h"

#include "Rendering/Image.h"
#include "Rendering/Shader.h"

#include <imgui.h>

BEGIN_CLASS_LINK(Texture)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

Texture::~Texture()
{
	GL(glDeleteTextures(1, &m_ID));
}

Texture* Texture::Create(ImageFormat format, ImageType type, unsigned int width, unsigned int height)
{
	Texture* texture = JingleScript::NewObject<Texture>("Texture");

	texture->m_ImageFormat = format;
	texture->m_ImageType = type;
	texture->m_Width = width;
	texture->m_Height = height;

	GL(glGenTextures(1, &texture->m_ID));

	texture->Bind();

	switch (type)
	{
		case ImageType::CUBEMAP:
			GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
			GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

			GL(glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, Image::FormatToGL(texture->m_ImageFormat), texture->m_Width, texture->m_Height));
			break;
		case ImageType::NORMAL:
		default:
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

			GL(glTexStorage2D(GL_TEXTURE_2D, 1, Image::FormatToGL(texture->m_ImageFormat), texture->m_Width, texture->m_Height));
			break;
	}

	texture->Unbind();

	return texture;
}

Texture* Texture::Create(Image* image)
{
	Texture* texture = JingleScript::NewObject<Texture>("Texture");

	texture->m_ImageFormat = image->m_ImageFormat;
	texture->m_ImageType = image->m_ImageType;
	texture->m_Width = image->m_Width;
	texture->m_Height = image->m_Height;

	GL(glGenTextures(1, &texture->m_ID));

	texture->Bind();
	
	switch (image->m_ImageType)
	{
		case ImageType::CUBEMAP:
			GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

			for (int i = 0; i < 6; i++)
			{
				GL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Image::FormatToGL(texture->m_ImageFormat), texture->m_Width, texture->m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->m_Data[i]));
			}
			break;
		case ImageType::NORMAL:
		default:
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

			GL(glTexImage2D(GL_TEXTURE_2D, 0, Image::FormatToGL(texture->m_ImageFormat), texture->m_Width, texture->m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->m_Data[0]));
			break;
	}

	texture->Unbind();

	return texture;
}

void Texture::Bind()
{
	GL(glBindTexture(m_ImageType == ImageType::CUBEMAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, m_ID));
}

void Texture::Bind(int index)
{
	GL(glActiveTexture(GL_TEXTURE0 + index));
	GL(glBindTexture(m_ImageType == ImageType::CUBEMAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, m_ID));
}

void Texture::Unbind()
{
	GL(glBindTexture(m_ImageType == ImageType::CUBEMAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, 0));
}

void Texture::ImGui(const ImVec2& size)
{
	ImGui::Image((ImTextureID)m_ID, size, { 0, 1 }, { 1, 0 });
}
