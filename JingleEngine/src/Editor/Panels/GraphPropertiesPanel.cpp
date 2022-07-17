#include "Editor/Panels/GraphPropertiesPanel.h"

#include "Core/Application.h"

#include "Editor/EditorModule.h"
#include "Editor/EditorUI.h"

#include "Editor/Editors/GraphEditor.h"

BEGIN_CLASS_LINK(GraphPropertiesPanel);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

void GraphPropertiesPanel::OnBeginRender(double DeltaTime)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
}

void GraphPropertiesPanel::OnEndRender(double DeltaTime)
{
	ImGui::PopStyleVar();
}

void GraphPropertiesPanel::OnRender(double DeltaTime)
{
	GraphEditor* editor = dynamic_cast<GraphEditor*>(GetEditor());
	if (!editor)
	{
		return;
	}

	EditorUI::ResetContext();

	if (!ImGui::BeginTable("GraphPropertiesPanel", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
	{
		return;
	}

	if (!(editor->SelectedEdges.size() > 1 || editor->SelectedNodes.size() > 1))
	{
		if (!editor->SelectedEdges.empty())
		{
			//editor->SelectedEdges[0]
		}
		else if (!editor->SelectedNodes.empty())
		{
			editor->SelectedNodes[0]->Editor_Render();
		}
	}

	ImGui::EndTable();
}
