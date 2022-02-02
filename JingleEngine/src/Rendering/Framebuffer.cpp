#include "Framebuffer.h"

#include "../Asset/AssetManager.h"

#include "Texture.h"

Framebuffer::Framebuffer(std::string name, const std::vector<TextureFormat>& attachments, unsigned int width, unsigned int height, bool cubeMap) : m_Name(name), m_AttachmentArray(attachments), m_CubeMap(cubeMap)
{
	Resize(width, height);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_ID);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	glViewport(0, 0, m_Width, m_Height);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Clear(unsigned int bits)
{
	glClear(bits);
}

void Framebuffer::Resize(unsigned int width, unsigned int height)
{
// MEMORY LEAK
	return;

	m_Width = width;
	m_Height = height;

	if (m_ID)
	{
		glDeleteFramebuffers(1, &m_ID);

		m_Attachments.clear();
		m_NumColorAttachment = 0;
	}

	glGenFramebuffers(1, &m_ID);

	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

	for (auto& attachment : m_AttachmentArray)
	{
		if (attachment == TextureFormat::DEPTH)
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
		glDrawBuffers(m_NumColorAttachment, buffers);
	}
	else
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		switch (status)
		{
		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cerr << "Framebuffer unsupported" << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cerr << "Framebuffer incomplete attachment" << std::endl;
			break;
		default:
			std::cerr << "Framebuffer error: " << status << std::endl;
			break;
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Ref<Texture> Framebuffer::GetTexture(int index)
{
	unsigned int attachment = GL_DEPTH_ATTACHMENT;
	if (index >= 0)
		attachment = GL_COLOR_ATTACHMENT0 + index;
	auto it = m_Attachments.find(attachment);
	if (it == m_Attachments.end())
		return nullptr;

	return it->second;
}

std::string Framebuffer::GetName()
{
	return m_Name;
}

bool Framebuffer::IsCubeMap()
{
	return m_CubeMap;
}

void Framebuffer::AttachColor(TextureFormat format)
{
	unsigned int attachment = GL_COLOR_ATTACHMENT0 + m_NumColorAttachment++;

	Ref<Texture> texture = AssetManager::Get<Texture>(GUID(m_Name + std::to_string(attachment)));
	texture->Create(format, m_Width, m_Height, m_CubeMap);
	m_Attachments[attachment] = texture;

	texture->Bind();

	if (m_CubeMap)
	{
		glFramebufferTexture(GL_FRAMEBUFFER, attachment, m_Attachments[attachment]->m_ID, 0);
	}
	else
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, m_Attachments[attachment]->m_ID, 0);
	}

	texture->Unbind();
}

void Framebuffer::AttachDepth(TextureFormat format)
{
	unsigned int attachment = GL_DEPTH_ATTACHMENT;

	Ref<Texture> texture = AssetManager::Get<Texture>(GUID(m_Name + std::to_string(attachment)));
	texture->Create(format, m_Width, m_Height, m_CubeMap);
	m_Attachments[attachment] = texture;

	texture->Bind();

	if (m_CubeMap)
	{
		glFramebufferTexture(GL_FRAMEBUFFER, attachment, m_Attachments[attachment]->m_ID, 0);
	}
	else
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, m_Attachments[attachment]->m_ID, 0);
	}

	texture->Unbind();
}
