#pragma once

#include "Editor/EditorPanel.h"

class GraphPanel : public EditorPanel
{
	DEFINE_CLASS(GraphPanel, EditorPanel);

	class Viewport* m_Viewport;

public:
	GraphPanel();

	virtual void OnBeginRender(double DeltaTime) override;
	virtual void OnRender(double DeltaTime) override;
	virtual void OnEndRender(double DeltaTime) override;

};
