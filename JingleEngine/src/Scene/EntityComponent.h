#pragma once

#include "Core/Core.h"

class Scene;
class Config;
class Entity;
class Application;

class EntityComponent : public JingleScript::Object
{
	DEFINE_CLASS(EntityComponent, JingleScript::Object);

	friend Entity;

private:
	Entity* m_Entity;

public:
	EntityComponent() {}

public:
	inline Entity* GetEntity() const { return m_Entity; }

};
