#pragma once

#include "Editor/EditorPanel.h"

class EntityHierarchyPanel : public EditorPanel
{
	DEFINE_CLASS(EntityHierarchyPanel, EditorPanel);

public:
	EntityHierarchyPanel() {}

	virtual void OnBeginRender(double DeltaTime) override;
	virtual void OnRender(double DeltaTime) override;
	virtual void OnEndRender(double DeltaTime) override;

protected:
	void RenderEntity(Entity* entity);

};
