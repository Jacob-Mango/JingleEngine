#pragma once

#include "Editor/EditorPanelBase.h"

#include <filesystem>

class ContentBrowserPanel : public EditorPanelBase
{
	DEFINE_CLASS(ContentBrowserPanel, EditorPanelBase);

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
