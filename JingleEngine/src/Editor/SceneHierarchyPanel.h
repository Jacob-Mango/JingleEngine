#pragma once

#include "Editor/EditorPanelBase.h"

class SceneHierarchyPanel : public EditorPanelBase
{
	DEFINE_CLASS(SceneHierarchyPanel, EditorPanelBase);

public:
	SceneHierarchyPanel() {}

	virtual void OnRender(double DeltaTime) override;

};
