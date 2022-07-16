#pragma once

#include "Core/Core.h"

#include "Asset/Asset.h"
#include "Asset/AssetModule.h"

#ifdef JE_EDITOR
	#include <imgui.h>
#endif

class Image;
class Shader;

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

class Texture : public JingleScript::ManagedObject
{
	DEFINE_CLASS(Texture, JingleScript::ManagedObject);

	friend class Framebuffer;

private:
	unsigned int m_Width;
	unsigned int m_Height;

	ImageFormat m_ImageFormat;
	ImageType m_ImageType;

	GLuint m_ID;

public:
	Texture() { }
	virtual ~Texture();

	static Texture* Create(Image* image);
	static Texture* Create(ImageFormat format, ImageType type, unsigned int width, unsigned int height);

	void Bind();
	void Bind(int index);
	void Unbind();

#ifdef JE_EDITOR
	void ImGui(const ImVec2& size);
#endif

};
