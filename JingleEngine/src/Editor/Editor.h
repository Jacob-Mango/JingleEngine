#pragma once

#include "Core/Core.h"
#include "Core/Module.h"

#include "Editor/EditorModule.h"

class Editor : public JingleScript::Object
{
	DEFINE_CLASS(Editor, JingleScript::Object);

	friend EditorModule;

private:
	bool m_ShouldClose = false;
	int m_OpenedIndex = 0;

	EditorModule* m_Editor;

public:
	Editor();
	~Editor();

	void OnRender(double DeltaTime);

	EditorModule* GetEditorModule() const { return m_Editor; }
	
	void Close();

};
