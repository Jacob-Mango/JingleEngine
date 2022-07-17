#pragma once

#include "Editor/EditorPanel.h"

class GraphPropertiesPanel : public EditorPanel
{
	DEFINE_CLASS(GraphPropertiesPanel, EditorPanel);

public:
	GraphPropertiesPanel() {}

	virtual void OnBeginRender(double DeltaTime) override;
	virtual void OnRender(double DeltaTime) override;
	virtual void OnEndRender(double DeltaTime) override;

};
