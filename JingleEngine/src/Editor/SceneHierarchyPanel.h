#pragma once

#include "Editor/EditorPanelBase.h"

class SceneHierarchyPanel : public EditorPanelBase
{
	DEFINE_CLASS(SceneHierarchyPanel, EditorPanelBase);

public:
	SceneHierarchyPanel() {}

	virtual void OnBeginRender(double DeltaTime) override;
	virtual void OnRender(double DeltaTime) override;
	virtual void OnEndRender(double DeltaTime) override;

};
