#pragma once

#include "Core/Core.h"
#include "Core/Application.h"

#include "Platform/Windows/WndWindow.h"

#ifdef _DEBUG
void RedirectIOToConsole();
#endif

int JingleEngineMain(Application* app);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
	RedirectIOToConsole();
#endif

	using namespace JingleScript;

	WndWindow::s_Instance = hInstance;

	//! Core scripts have to be compiled before application initialization!
	//! This is because Application inherits from JingleScript::Object and the 
	//!  virtual table is created when the scripts are compiled.
	//! A later revision may retroactiviely update the virtual tables of already 
	//!  allocated objects but that requires tracking.
	std::string folder = "Assets";

	Ref<Parser> parser = new Parser();
	bool success = parser->ParseFiles(folder + "/Scripts/");
	parser->SerializeNodes(folder + "/compiled.jst");

	if (success)
	{
		success &= Compiler::Compile(parser);
		Globals::Output(folder + "/Compiled.jsi");
	}

	if (!success)
	{
		return -1;
	}

	Application* application = JingleScript::NewObject<Application>("Application");

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
