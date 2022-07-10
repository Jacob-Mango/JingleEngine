#pragma once

#include "Core/Core.h"
#include "Core/Module.h"

#include "Editor/EditorAttribute.h"

class Editor;
class EditorModule;

struct EditorPanelData;

class EditorPanel : public JingleScript::Object
{
	DEFINE_CLASS(EditorPanel, JingleScript::Object);

	friend Editor;
	friend EditorModule;

private:
	bool m_ShouldClose = false;

	EditorModule* m_Module;
	Editor* m_Editor;

public:
	EditorPanel();
	~EditorPanel();

public:
	static bool Render(EditorPanel* panel, double DeltaTime, const std::string& title, ImGuiWindowFlags flags);

	virtual void OnBeginRender(double DeltaTime);
	virtual void OnRender(double DeltaTime);
	virtual void OnEndRender(double DeltaTime);

public:
	EditorModule* GetEditorModule() const { return m_Module; }
	Editor* GetEditor() const { return m_Editor; }
	
public:
	void Close();

};

struct EditorPanelData
{
	EditorAttribute* Attribute;

	std::vector<EditorPanel*> Instances;
};
