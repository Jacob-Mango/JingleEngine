class EditorModule extends Module
{

}

class EditorAttribute extends Attribute
{
	string m_Title;

	EditorAttribute(string title)
	{
		m_Title = title;
	}
}

class EditorPanelBase
{
	void OnRender(double DeltaTime)
	{
	}
}

@EditorAttribute("Viewport")
class EditorViewportPanel extends EditorPanelBase
{

}

@EditorAttribute("Scene Hierarchy")
class SceneHierarchyPanel extends EditorPanelBase
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

		Scene scene = app.GetScene();
		ImGui_Text(scene);
		Camera camera = scene.GetCamera();
		ImGui_Text(camera);

		// null comparison broken??
		//if (camera)
		{
			double x = camera.PositionX;
			double y = camera.PositionY;
			double z = camera.PositionZ;

			ImGui_Text("PositionX: " + x);
			ImGui_Text("PositionY: " + y);
			ImGui_Text("PositionZ: " + z);
		}
	}
}
