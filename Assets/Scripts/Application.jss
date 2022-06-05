class Application
{
	int FPS;
	
	void OnUpdate(double DeltaTime)
	{
		ImGui_Begin("Test");

		ImGui_Text("This is jingle script");
		//ImGui_Text("DeltaTime: " + DeltaTime);
		//ImGui_Text("FPS: " + FPS);

		ImGui_End();
	}
}

@Native
Application GetApplication();
