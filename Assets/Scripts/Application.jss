class Application
{
	int FPS;
	
	void OnUpdate(double DeltaTime)
	{
	}

	@Native
	Scene GetScene();
}

@Native
Application GetApplication();
