#pragma once

#include "Core/Core.h"

#include "Scene/Scene.h"

#include "Rendering/Framebuffer.h"

//! TODO: Maybe have 'Viewport' inherit from 'Framebuffer'

/**
 * A viewport will contain a reference to the Scene and a Camera entity
 * 
 * It will also contain a reference to the Renderer module and Framebuffer
 */
class Viewport : public JingleScript::ManagedObject
{
	DEFINE_CLASS(Viewport, JingleScript::ManagedObject);

	friend class Renderer;

	unsigned int m_Width;
	unsigned int m_Height;

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;

	glm::vec3 m_CameraPosition;
	
	Ref<Scene> m_Scene;
	Ref<Framebuffer> m_Framebuffer;

public:
	Viewport() {}

	void Process(double DeltaTime);

	void Resize(unsigned int width, unsigned int height);

	Texture* GetTexture() const;

	Scene* GetScene() const { return m_Scene; }
	void SetScene(Scene* scene) { m_Scene = scene; }

public:
	static Viewport* Create(Scene* scene, unsigned int width, unsigned int height);

};
