#include "Editor/EditorPanel.h"

BEGIN_MODULE_LINK(EditorPanel);
END_MODULE_LINK();

void EditorPanel::OnEvent(BaseClass* sender, const EventArgs& args)
{
	switch (args.GetType())
	{
		case EventTypeImGuiRender:
			if (ImGui::Begin("Scene Hierarchy", &m_IsOpen))
			{
				OnRender(args.As<ImGuiRenderEventArgs>().DeltaTime);

				ImGui::End();
			}
			
			break;
	}
}

std::string EditorPanel::GetTitle() const
{
	return "Editor Panel";
}

void EditorPanel::Open()
{
	m_IsOpen = true;
}

void EditorPanel::Close()
{
	m_IsOpen = false;
}

bool EditorPanel::IsOpen() const
{
	return m_IsOpen;
}
