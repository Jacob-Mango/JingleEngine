#pragma once

#include "../Core/Core.h"

class Shader;
class Texture;
enum class TextureFormat;

class Framebuffer : public Countable
{
private:
	std::string m_Name;

	GLuint m_ID;
	GLuint m_RBOID;
	unsigned int m_Width;
	unsigned int m_Height;

	bool m_CubeMap;

	unsigned int m_NumColorAttachment;
	std::map<unsigned int, Ref<Texture>> m_Attachments;

	std::vector<TextureFormat> m_AttachmentArray;

public:
	Framebuffer(std::string name, const std::vector<TextureFormat>& attachments, unsigned int width = 512, unsigned int height = 512, bool cubeMap = false);
	virtual ~Framebuffer();

	void Bind();
	void Unbind();

	void Clear(unsigned int bits);

	void Resize(unsigned int width, unsigned int height);

	Ref<Texture> GetTexture(int index);
	std::string GetName();

	bool IsCubeMap();

private:
	void AttachColor(TextureFormat format);
	void AttachDepth(TextureFormat format);
};
