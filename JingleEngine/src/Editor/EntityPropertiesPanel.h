#pragma once

#include "Editor/EditorPanelBase.h"

class EntityPropertiesPanel : public EditorPanelBase
{
	DEFINE_CLASS(EntityPropertiesPanel, EditorPanelBase);

public:
	EntityPropertiesPanel() {}

	virtual void OnBeginRender(double DeltaTime) override;
	virtual void OnRender(double DeltaTime) override;
	virtual void OnEndRender(double DeltaTime) override;

};
