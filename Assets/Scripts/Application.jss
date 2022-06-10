class Application
{
	int FPS;
	
	void OnUpdate(double DeltaTime)
	{
		ImGui_Begin("Test");

		ImGui_Text("This is jingle script");
		ImGui_Text("DeltaTime: " + DeltaTime);
		ImGui_Text("FPS: " + FPS);

		bool b1 = ImGui_WantCaptureKeyboard();
		bool b2 = ImGui_WantCaptureMouse();

		ImGui_Text("WantCaptureKeyboard: " + b1);
		ImGui_Text("WantCaptureMouse: " + b2);

		Scene scene = this.GetScene();
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

		ImGui_End();
	}

	@Native
	Scene GetScene();
}

@Native
Application GetApplication();
