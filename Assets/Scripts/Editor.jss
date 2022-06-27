class EditorModule extends Module
{

}

class EditorAttribute extends Attribute
{
	string m_Title;

	@Native
	EditorAttribute(string title);
}

class EditorPanelBase
{
	@Native
	void OnBeginRender(double DeltaTime);

	@Native
	void OnRender(double DeltaTime);

	@Native
	void OnEndRender(double DeltaTime);
}

@EditorAttribute("Viewport")
class EditorViewportPanel extends EditorPanelBase
{

}

@EditorAttribute("Scene Hierarchy")
class SceneHierarchyPanel extends EditorPanelBase
{

}

@EditorAttribute("Entity Properties")
class EntityPropertiesPanel extends EditorPanelBase
{

}

@EditorAttribute("Script Test")
class ScriptTestPanel extends EditorPanelBase
{
	void OnRender(double DeltaTime)
	{
		Application app = GetApplication();

		ImGui_Text("This is jingle script");
		ImGui_Text("DeltaTime: " + DeltaTime);
		ImGui_Text("FPS: " + app.FPS);

		bool b1 = ImGui_WantCaptureKeyboard();
		bool b2 = ImGui_WantCaptureMouse();

		ImGui_Text("WantCaptureKeyboard: " + b1);
		ImGui_Text("WantCaptureMouse: " + b2);
	}
}
