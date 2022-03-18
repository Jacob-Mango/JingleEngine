#include "Core/ModuleManager.h"
#include "Core/Module.h"

#include <algorithm>

#include <JingleScript.h>

void ModuleManager::Initialize()
{
	using namespace JingleScript;

	ModuleManager& manager = Get();

	std::vector<std::string> modulesToLoad;

	{
		std::ifstream fileStream("Assets/modules.txt");

		std::string line;
		while (std::getline(fileStream, line))
		{
			modulesToLoad.push_back(line);
		}

		fileStream.close();
	}

	for (auto& name : modulesToLoad)
	{
		Type* type = TypeManager::Get(name);
		if (type == nullptr)
		{
			JS_CRITICAL("Invalid module %s", name.c_str());
			continue;
		}

		Module* module = NewObject<Module>(name);
		while (type != nullptr)
		{
			manager.m_ModuleMap[type->Name()] = module;
			type = type->GetBase();
		}
		manager.m_Modules.push_back(module);
	}
}

ModuleManager& ModuleManager::Get()
{
	static ModuleManager manager;
	return manager;
}

ModuleIterator& ModuleManager::Iterator()
{
	return Get().m_Iterator;
}

void ModuleManager::On(std::function<void(Module*)> func)
{
	for (auto module : Get().m_Modules)
	{
		func(module);
	}
}

std::vector<Module*>::iterator ModuleIterator::begin()
{
	return ModuleManager::Get().m_Modules.begin();
}

std::vector<Module*>::iterator ModuleIterator::end()
{
	return ModuleManager::Get().m_Modules.end();
}