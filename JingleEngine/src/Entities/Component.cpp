#include "Entities/Component.h"

BEGIN_CLASS_LINK(Component)
	LINK_CONSTRUCTOR();
	LINK_CONSTRUCTOR(Entity*);
	LINK_METHOD(OnCreate);
	LINK_METHOD(OnDestroy);
	LINK_METHOD(OnTick);
END_CLASS_LINK()

Component::Component(Entity* entity) : m_Entity(entity)
{
}

void Component::OnCreate()
{
}

void Component::OnDestroy()
{
}

void Component::OnTick(double DeltaTime)
{
}
