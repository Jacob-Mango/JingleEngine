#pragma once

#include "Editor/EditorPanel.h"

class EntityPropertiesPanel : public EditorPanel
{
	DEFINE_CLASS(EntityPropertiesPanel, EditorPanel);

public:
	EntityPropertiesPanel() {}

	virtual void OnBeginRender(double DeltaTime) override;
	virtual void OnRender(double DeltaTime) override;
	virtual void OnEndRender(double DeltaTime) override;

};
