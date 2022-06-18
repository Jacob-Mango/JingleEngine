#pragma once

#include "Core/Core.h"

#include "Rendering/Image.h"
#include "Rendering/Texture.h"

class Framebuffer : public JingleScript::ManagedObject
{
	DEFINE_CLASS(Framebuffer, JingleScript::ManagedObject);

private:
	GLuint m_ID;
	GLuint m_RBOID;
	unsigned int m_Width;
	unsigned int m_Height;

	ImageType m_ImageType;

	unsigned int m_NumColorAttachment;
	std::map<unsigned int, Ref<Texture>> m_Attachments;

	std::vector<ImageFormat> m_AttachmentArray;

public:
	Framebuffer() {}
	virtual ~Framebuffer();

	static Framebuffer* Create(const std::vector<ImageFormat>& attachments, ImageType type, unsigned int width, unsigned int height);

	void Bind();
	void Unbind();

	void Resize(unsigned int width, unsigned int height);

	Texture* GetTexture(int index);
	ImageType GetImageType();

private:
	void AttachColor(ImageFormat format);
	void AttachDepth(ImageFormat format);

};
