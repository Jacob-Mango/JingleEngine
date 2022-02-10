#include "Core/ModuleManager.h"

ModuleManager* ModuleManager::s_Instance;

void ModuleManager::Initialize()
{
	if (s_Instance != nullptr) return;

	s_Instance = new ModuleManager();
}

ModuleIterator& ModuleManager::Iterator()
{
	return s_Instance->m_Iterator;
}

void ModuleManager::On(std::function<void(Module*)> func)
{
	for (auto module : s_Instance->m_Modules)
	{
		func(module);
	}
}

std::vector<Module*>::iterator ModuleIterator::begin()
{
	return ModuleManager::s_Instance->m_Modules.begin();
}

std::vector<Module*>::iterator ModuleIterator::end()
{
	return ModuleManager::s_Instance->m_Modules.end();
}
