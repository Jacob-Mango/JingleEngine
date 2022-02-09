#include "Core.h"

extern int JingleEngineMain(Application* app);

int main(int argc, char** argv)
{
	Application* application = new Application();

	int error;

	if ((error = application->Init()) != 0)
	{
		return error;
	}

	application->RegisterModule<AssetModule>();
	application->RegisterModule<Renderer>();

	if ((error = JingleEngineMain(application)) != 0)
	{
		return error;
	}

	application->Run();
	return 0;
}
