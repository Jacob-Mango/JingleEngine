#pragma once

#include "Core/Core.h"

#include "Asset/Asset.h"
#include "Asset/AssetModule.h"

class Shader;

enum class TextureFormat
{
	RGBA8,
	RGBA16,
	RGBA32,
	DEPTH
};

class Texture : public Asset
{
	SETUP_ASSET(Texture, Asset);

	friend class Framebuffer;
	friend class Scene;

private:
	unsigned int m_Width;
	unsigned int m_Height;

	TextureFormat m_Format;
	bool m_CubeMap;

	GLuint m_ID;

public:
	Texture();
	virtual ~Texture();

	static GLenum FormatToGL(TextureFormat format);

	void Create(TextureFormat format, unsigned int width, unsigned int height, bool cubeMap);

	void Bind();
	void Bind(int index);
	void Unbind();

public:
	virtual bool OnLoad() override;
};
