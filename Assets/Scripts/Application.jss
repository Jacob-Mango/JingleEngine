class Application
{
	int FPS;
	
	void OnUpdate(double DeltaTime)
	{
	}

	@Native
	Entity GetScene();
}

@Native
Application GetApplication();
