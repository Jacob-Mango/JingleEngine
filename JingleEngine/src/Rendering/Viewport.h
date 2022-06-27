#pragma once

#include "Core/Core.h"

#include "Entities/Entity.h"

#include "Rendering/Framebuffer.h"

class Viewport : public JingleScript::ManagedObject
{
	DEFINE_CLASS(Viewport, JingleScript::ManagedObject);

	friend class Renderer;

	unsigned int m_Width;
	unsigned int m_Height;

protected:
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;

	glm::vec3 m_CameraPosition;
	
	Entity* m_Scene;
	Ref<Framebuffer> m_Framebuffer;

public:
	Viewport() {}

	virtual void Process(double DeltaTime);

	void Resize(unsigned int width, unsigned int height);

	Texture* GetTexture() const;

	Entity* GetScene() const { return m_Scene; }
	void SetScene(Entity* scene) { m_Scene = scene; }

public:
	static Viewport* Create(std::string type, Entity* scene, unsigned int width, unsigned int height);

};
