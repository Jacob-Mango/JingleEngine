#pragma once

#include "Editor/EditorPanel.h"

class SceneHierarchyPanel : public EditorPanel
{
	DEFINE_MODULE(SceneHierarchyPanel, EditorPanel);

public:
	SceneHierarchyPanel() {}

	virtual void OnRender(double DeltaTime) override;

	virtual std::string GetTitle() const override;
};
