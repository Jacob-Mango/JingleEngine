#include "Rendering/Framebuffer.h"

#include "Asset/AssetModule.h"

#include "Rendering/Texture.h"

BEGIN_CLASS_LINK(Framebuffer)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

Framebuffer::~Framebuffer()
{
	GL(glDeleteFramebuffers(1, &m_ID));
}

Framebuffer* Framebuffer::Create(const std::vector<ImageFormat>& attachments, ImageType type, unsigned int width, unsigned int height)
{
	Framebuffer* framebuffer = JingleScript::NewObject<Framebuffer>("Framebuffer");

	framebuffer->m_AttachmentArray = attachments;
	framebuffer->m_ImageType = type;
	framebuffer->Resize(width, height);

	return framebuffer;
}

void Framebuffer::Bind()
{
	GL(glBindFramebuffer(GL_FRAMEBUFFER, m_ID));
	GL(glViewport(0, 0, m_Width, m_Height));
}

void Framebuffer::Unbind()
{
	GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::Resize(unsigned int width, unsigned int height)
{
	if (m_Width == width && m_Height == height)
	{
		return;
	}

	m_Width = width;
	m_Height = height;

	if (m_ID)
	{
		GL(glDeleteFramebuffers(1, &m_ID));

		m_Attachments.clear();
		m_NumColorAttachment = 0;
	}

	GL(glGenFramebuffers(1, &m_ID));

	GL(glBindFramebuffer(GL_FRAMEBUFFER, m_ID));

	for (auto& attachment : m_AttachmentArray)
	{
		if (attachment == ImageFormat::DEPTH)
		{
			AttachDepth(attachment);
		}
		else
		{
			AttachColor(attachment);
		}
	}

	if (m_NumColorAttachment > 0)
	{
		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		GL(glDrawBuffers(m_NumColorAttachment, buffers));
	}
	else
	{
		GL(glDrawBuffer(GL_NONE));
		GL(glReadBuffer(GL_NONE));
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		switch (status)
		{
		case GL_FRAMEBUFFER_UNSUPPORTED:
			JS_ERROR("OpenGL Error ({}): Framebuffer unsupported", status);
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			JS_ERROR("OpenGL Error ({}): Framebuffer has incomplete attachment", status);
			break;
		default:
			JS_ERROR("OpenGL Error ({}): Framebuffer error", status);
			break;
		}
	}

	GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

Texture* Framebuffer::GetTexture(int index)
{
	unsigned int attachment = GL_DEPTH_ATTACHMENT;
	if (index >= 0)
		attachment = GL_COLOR_ATTACHMENT0 + index;
	auto it = m_Attachments.find(attachment);
	if (it == m_Attachments.end())
		return nullptr;

	return it->second;
}

ImageType Framebuffer::GetImageType()
{
	return m_ImageType;
}

void Framebuffer::AttachColor(ImageFormat format)
{
	unsigned int attachment = GL_COLOR_ATTACHMENT0 + m_NumColorAttachment++;

	Texture* texture = Texture::Create(format, m_ImageType, m_Width, m_Height);
	m_Attachments[attachment] = texture;

	texture->Bind();

	if (m_ImageType == ImageType::CUBEMAP)
	{
		GL(glFramebufferTexture(GL_FRAMEBUFFER, attachment, m_Attachments[attachment]->m_ID, 0));
	}
	else
	{
		GL(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, m_Attachments[attachment]->m_ID, 0));
	}

	texture->Unbind();
}

void Framebuffer::AttachDepth(ImageFormat format)
{
	unsigned int attachment = GL_DEPTH_ATTACHMENT;

	Texture* texture = Texture::Create(format, m_ImageType, m_Width, m_Height);
	m_Attachments[attachment] = texture;

	texture->Bind();

	if (m_ImageType == ImageType::CUBEMAP)
	{
		GL(glFramebufferTexture(GL_FRAMEBUFFER, attachment, m_Attachments[attachment]->m_ID, 0));
	}
	else
	{
		GL(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, m_Attachments[attachment]->m_ID, 0));
	}

	texture->Unbind();
}
