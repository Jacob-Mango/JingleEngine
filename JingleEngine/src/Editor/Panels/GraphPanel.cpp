#include "Editor/Panels/GraphPanel.h"

BEGIN_CLASS_LINK(GraphPanel);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

GraphPanel::GraphPanel()
{
}

void GraphPanel::OnBeginRender(double DeltaTime)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
}

void GraphPanel::OnRender(double DeltaTime)
{
}

void GraphPanel::OnEndRender(double DeltaTime)
{
	ImGui::PopStyleVar();
}
