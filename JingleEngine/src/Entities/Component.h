#pragma once

#include "Core/Core.h"

#include "Entities/Entity.h"

class Config;
class Application;

class Component : public JingleScript::Object, public ObjectProperty
{
	DEFINE_CLASS(Component, JingleScript::Object);

	friend Entity;

private:
	Entity* m_Entity = nullptr;

public:
	Component() {}
	Component(Entity* entity);

public:
	inline Entity* GetEntity() const { return m_Entity; }

	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnTick(double DeltaTime);

	virtual void Editor_OnPropertyChanged(std::string name) override;

};

class ComponentArray : public JingleScript::Array<Component*>
{
	DEFINE_CLASS(ComponentArray, JingleScript::Array<Component*>);

public:
	ComponentArray() {}
	~ComponentArray() {}

};
