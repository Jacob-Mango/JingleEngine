#include "Editor/EditorPanelBase.h"

BEGIN_CLASS_LINK(EditorPanelBase);
	LINK_CONSTRUCTOR();
	LINK_METHOD(OnBeginRender);
	LINK_METHOD(OnRender);
	LINK_METHOD(OnEndRender);
	LINK_METHOD(GetEditor);
	LINK_METHOD(Close);
END_CLASS_LINK();

EditorPanelBase::EditorPanelBase()
{
	m_Editor = ModuleManager::Get<EditorModule>();
}

EditorPanelBase::~EditorPanelBase()
{
}

void EditorPanelBase::OnBeginRender(double DeltaTime)
{
}

void EditorPanelBase::OnRender(double DeltaTime)
{
}

void EditorPanelBase::OnEndRender(double DeltaTime)
{
}

void EditorPanelBase::OnEvent(BaseClass* sender, const EventArgs& args)
{
}

void EditorPanelBase::Close()
{
	m_ShouldClose = false;
}

