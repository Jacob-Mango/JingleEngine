#include <imgui.h>

#include <JingleScript.h>

bool ImGui_Begin(std::string name)
{
	return ImGui::Begin(name.c_str());
}

void ImGui_End()
{
	ImGui::End();
}

void ImGui_Text(std::string text)
{
	ImGui::Text(text.c_str());
}

// Temporarily called in 'Application::Initialize' until static functions exist in classes
void LinkImGUI()
{
	LINK_FUNCTION(ImGui_Begin);
	LINK_FUNCTION(ImGui_End);

	LINK_FUNCTION(ImGui_Text);
}
