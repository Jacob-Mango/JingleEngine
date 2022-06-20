#pragma once

#include "Editor/EditorPanelBase.h"

class EditorViewportPanel : public EditorPanelBase
{
	DEFINE_CLASS(EditorViewportPanel, EditorPanelBase);

	class Viewport* m_Viewport;

public:
	EditorViewportPanel();

	virtual void OnBeginRender(double DeltaTime) override;
	virtual void OnRender(double DeltaTime) override;
	virtual void OnEndRender(double DeltaTime) override;

};
