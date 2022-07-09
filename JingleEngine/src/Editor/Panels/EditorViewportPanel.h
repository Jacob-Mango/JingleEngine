#pragma once

#include "Editor/EditorPanel.h"

class EditorViewportPanel : public EditorPanel
{
	DEFINE_CLASS(EditorViewportPanel, EditorPanel);

	class Viewport* m_Viewport;

public:
	EditorViewportPanel();

	virtual void OnBeginRender(double DeltaTime) override;
	virtual void OnRender(double DeltaTime) override;
	virtual void OnEndRender(double DeltaTime) override;

};
