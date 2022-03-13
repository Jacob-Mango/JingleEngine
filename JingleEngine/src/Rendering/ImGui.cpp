#include <imgui.h>

#include <JingleScript.h>

bool ImGui_Begin(const char* name)
{
	return ImGui::Begin(name);
}

void ImGui_End()
{
	ImGui::End();
}

void ImGui_Text(const char* text)
{
	ImGui::Text(text);
}

// Temporarily called in 'Application::Initialize' until static functions exist in classes
void LinkImGUI()
{
	LINK_FUNCTION(ImGui_Begin);
	LINK_FUNCTION(ImGui_End);

	LINK_FUNCTION(ImGui_Text);
}
