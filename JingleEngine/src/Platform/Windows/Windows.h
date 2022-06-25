#pragma once

#include "Core/Core.h"
#include "Core/Application.h"

#include "Platform/Windows/WndWindow.h"

int JingleEngineMain(Application* app);

int main(int argc, char** argv)
{
	using namespace JingleScript;

	//! TODO: Project files are loaded before application initialization. 

	//! Core scripts have to be compiled before application initialization!
	//! This is because Application inherits from JingleScript::Object and the 
	//!  virtual table is created when the scripts are compiled.
	//! A later revision may retroactiviely update the virtual tables of already 
	//!  allocated objects but that requires tracking.
	std::string folder = "Assets";

	std::string projectName = "TestProject";

	Ref<Parser> parser = new Parser();
	bool success = parser->ParseFiles(folder + "/Scripts/");
	parser->SerializeNodes(projectName + ".jst");

	if (success)
	{
		success &= Compiler::Compile(parser);
	}

	Globals::Output(projectName + ".jsi");

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
