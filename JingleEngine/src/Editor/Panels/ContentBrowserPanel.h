#pragma once

#include "Editor/EditorPanel.h"

#include <filesystem>

class ContentBrowserPanel : public EditorPanel
{
	DEFINE_CLASS(ContentBrowserPanel, EditorPanel);

private:
	std::filesystem::path m_CurrentDirectory;
	std::filesystem::path m_RootPath;
	bool m_PathChanged;

public:
	ContentBrowserPanel();

	virtual void OnBeginRender(double DeltaTime) override;
	virtual void OnRender(double DeltaTime) override;
	virtual void OnEndRender(double DeltaTime) override;

};
