#pragma once

#include "Core/Core.h"

#include "Core/Logging.h"
#include "Core/Module.h"

class EditorPanelBase : public JingleScript::Object
{
	DEFINE_CLASS(EditorPanelBase, JingleScript::Object);

	friend class EditorModule;

private:
	bool m_ShouldClose = false;
	int m_OpenedIndex = 0;

public:
	EditorPanelBase();
	~EditorPanelBase();

	virtual void OnRender(double DeltaTime);

	virtual void OnEvent(BaseClass* sender, const EventArgs& args);
	
	void Close();

};
