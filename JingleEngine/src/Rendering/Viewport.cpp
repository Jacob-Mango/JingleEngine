#include "Rendering/Viewport.h"

#include "Rendering/Renderer.h"

BEGIN_CLASS_LINK(Viewport)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

Viewport* Viewport::Create(std::string type, Entity* scene, unsigned int width, unsigned int height)
{
	Viewport* viewport = JingleScript::NewObject<Viewport>(type);
	if (!viewport)
	{
		JS_ERROR("Failed to create viewport of type {}", type);
		return nullptr;
	}
	
	viewport->m_Scene = scene;
	viewport->m_Width = width;
	viewport->m_Height = height;

	viewport->m_Framebuffer = Framebuffer::Create({ ImageFormat::RGBA32 }, ImageType::NORMAL, width, height);

	return viewport;
}

void Viewport::Resize(unsigned int width, unsigned int height)
{
	m_Width = width;
	m_Height = height;

	m_Framebuffer->Resize(m_Width, m_Height);
}

void Viewport::Process(double DeltaTime)
{
	Renderer* renderer = ModuleManager::Get<Renderer>();
	if (m_Width == 0 || m_Height == 0)
	{
		return;
	}

	float aspect = (float)m_Width / (float)m_Height;
	m_ProjectionMatrix = glm::perspective(glm::radians(90.0f), aspect, 0.001f, 1000.0f);

	m_Framebuffer->Bind();

	renderer->Process(DeltaTime, this);

	m_Framebuffer->Unbind();
}

Texture* Viewport::GetTexture() const
{
	return m_Framebuffer->GetTexture(0);
}
