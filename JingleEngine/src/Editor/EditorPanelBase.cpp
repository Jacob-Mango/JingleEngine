#include "Editor/EditorPanelBase.h"

BEGIN_CLASS_LINK(EditorPanelBase);
END_CLASS_LINK();

EditorPanelBase::EditorPanelBase()
{
}

EditorPanelBase::~EditorPanelBase()
{
}

void EditorPanelBase::OnRender(double DeltaTime)
{
}

void EditorPanelBase::OnEvent(BaseClass* sender, const EventArgs& args)
{
}

void EditorPanelBase::Close()
{
	m_ShouldClose = false;
}

