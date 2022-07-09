#pragma once

#include "Core/Core.h"
#include "Core/Module.h"

#include "Editor/EditorAttribute.h"

class Editor;
class EditorModule;

class EditorPanel : public JingleScript::Object
{
	DEFINE_CLASS(EditorPanel, JingleScript::Object);

	friend Editor;
	friend EditorModule;

private:
	bool m_ShouldClose = false;

	EditorModule* m_Editor;

public:
	EditorPanel();
	~EditorPanel();

	virtual void OnBeginRender(double DeltaTime);
	virtual void OnRender(double DeltaTime);
	virtual void OnEndRender(double DeltaTime);

	virtual void OnEvent(BaseClass* sender, const EventArgs& args);

	EditorModule* GetEditor() const { return m_Editor; }
	
	void Close();

};

struct EditorPanelData
{
	EditorAttribute* Attribute;

	std::vector<EditorPanel*> Instances;
};
