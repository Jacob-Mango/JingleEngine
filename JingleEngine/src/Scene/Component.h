#pragma once

#include "Core/Core.h"

#include "Scene/Entity.h"

class Scene;
class Config;
class Application;

class Component : public JingleScript::Object
{
	DEFINE_CLASS(Component, JingleScript::Object);

	friend Entity;

private:
	Entity* m_Entity;

public:
	Component() {}
	Component(Entity* entity);

public:
	inline Entity* GetEntity() const { return m_Entity; }

	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnTick(double DeltaTime);

};
