class EditorModule extends Module
{

}

class EditorAttribute extends Attribute
{
	string m_Title;

	@Native
	EditorAttribute(string title);
}

class EditorPanel
{
	@Native
	void OnBeginRender(double DeltaTime);

	@Native
	void OnRender(double DeltaTime);

	@Native
	void OnEndRender(double DeltaTime);
}

@EditorAttribute("Viewport")
class EditorViewportPanel extends EditorPanel
{

}

@EditorAttribute("Scene Hierarchy")
class EntityHierarchyPanel extends EditorPanel
{

}

@EditorAttribute("Entity Properties")
class EntityPropertiesPanel extends EditorPanel
{

}

@EditorAttribute("Content Browser")
class ContentBrowserPanel extends EditorPanel
{

}

@EditorAttribute("Graph")
class GraphPanel extends EditorPanel
{

}

@EditorAttribute("Script Test")
class ScriptTestPanel extends EditorPanel
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

class Editor
{
}

@EditorAttribute("Entity Editor")
class EntityEditor extends Editor
{
}

@EditorAttribute("Shader Editor")
class ShaderEditor extends Editor
{
}
