#include "Rendering/Image.h"
#include "Rendering/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

BEGIN_CLASS_LINK(Image)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

Image::~Image()
{
	for (int i = 0; i < m_NumImages; i++)
	{
		stbi_image_free(m_Data[i]);
	}

	delete [] m_Data;
}

void Image::Bind()
{
	m_Texture->Bind();
}

void Image::Bind(int index)
{
	m_Texture->Bind(index);
}

void Image::Unbind()
{
	m_Texture->Unbind();
}

GLenum Image::FormatToGL(ImageFormat format)
{
	switch (format)
	{
	case ImageFormat::RGBA8:
		return GL_RGB8;
	case ImageFormat::RGBA16:
		return GL_RGBA16F;
	case ImageFormat::RGBA32:
		return GL_RGBA32F;
	case ImageFormat::DEPTH:
		return GL_DEPTH_COMPONENT32F;
	}
	return 0;
}

bool Image::OnLoad()
{
	if (GetPath() == "")
		return false;

	std::string path = GetPath();
	std::string extension = GetExtension();

	stbi_set_flip_vertically_on_load(true);
	int components;

	if (extension.compare("cubemap") == 0)
	{
		path = path.substr(0, path.size() - 7);

		m_NumImages = 6;
		m_Data = new unsigned char*[m_NumImages];
		m_ImageFormat = ImageFormat::RGBA16;
		m_ImageType = ImageType::CUBEMAP;

		for (int i = 0; i < m_NumImages; i++)
		{
			m_Data[i] = stbi_load((path + std::to_string(i) + ".png").c_str(), (int*)&m_Width, (int*)&m_Height, &components, STBI_rgb);
		}

		m_Texture = Texture::Create(this);

		return true;
	}

	if (extension.compare("png") == 0)
	{
		m_NumImages = 1;
		m_Data = new unsigned char*[m_NumImages];
		m_ImageFormat = ImageFormat::RGBA16;
		m_ImageType = ImageType::NORMAL;

		m_Data[0] = stbi_load(path.c_str(), (int*)&m_Width, (int*)&m_Height, &components, STBI_rgb);

		m_Texture = Texture::Create(this);

		return true;
	}

	return false;
}
