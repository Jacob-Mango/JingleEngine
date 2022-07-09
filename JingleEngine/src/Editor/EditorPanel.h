#pragma once

#include "Core/Core.h"
#include "Core/Module.h"

#include "Editor/EditorModule.h"

class EditorPanel : public JingleScript::Object
{
	DEFINE_CLASS(EditorPanel, JingleScript::Object);

	friend EditorModule;

private:
	bool m_ShouldClose = false;
	int m_OpenedIndex = 0;

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
