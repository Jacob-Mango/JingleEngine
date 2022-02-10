#include "Core.h"

#include "Platform/SDL/SDLWindow.h"

int JingleEngineMain(Application* app);

int main(int argc, char** argv)
{
	Application* application = new Application();

	int error;

	if ((error = application->Initialize()) != 0)
	{
		return error;
	}

	if ((error = JingleEngineMain(application)) != 0)
	{
		return error;
	}

	application->Run();
	return 0;
}
