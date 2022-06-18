#pragma once

#include "Core/Core.h"

#include "Asset/Asset.h"
#include "Asset/AssetModule.h"

class Shader;
class Texture;

enum class ImageFormat
{
	RGBA8,
	RGBA16,
	RGBA32,
	DEPTH
};

enum class ImageType
{
	NORMAL,
	CUBEMAP
};

class Image : public Asset
{
	SETUP_ASSET(Image, Asset);

	friend class Texture;
	friend class Scene;

private:
	unsigned int m_Width;
	unsigned int m_Height;

	ImageFormat m_ImageFormat;
	ImageType m_ImageType;

	unsigned int m_NumImages;
	unsigned char** m_Data;

	Ref<Texture> m_Texture;

public:
	Image() {}
	virtual ~Image();

	void Bind();
	void Bind(int index);
	void Unbind();

	virtual bool OnLoad() override;

	static GLenum FormatToGL(ImageFormat format);

};
