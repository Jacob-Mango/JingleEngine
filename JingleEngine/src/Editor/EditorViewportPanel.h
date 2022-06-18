#pragma once

#include "Editor/EditorPanel.h"

class EditorViewportPanel : public EditorPanel
{
	DEFINE_MODULE(EditorViewportPanel, EditorPanel);

	class Viewport* m_Viewport;

public:
	EditorViewportPanel() {}

	virtual void OnInitialize() override;

	virtual void OnRender(double DeltaTime) override;

	virtual std::string GetTitle() const override;
};
