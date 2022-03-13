#pragma once

#include <JingleScript.h>

#include <functional>
#include <vector>

class Module;

class ModuleIterator
{
    using ModuleList = std::vector<Module*>;

public:
    ModuleList::iterator begin();
    ModuleList::iterator end();
};

class ModuleManager : public BaseClass
{
	friend class ModuleIterator;

private:
	static ModuleManager* s_Instance;

private:
	std::unordered_map<std::string, Module*> m_ModuleMap;
	std::vector<Module*> m_Modules;
	ModuleIterator m_Iterator;

public:
	static void Initialize();

	static ModuleIterator& Iterator();

	static void On(std::function<void(Module*)> func);

	template<typename T>
	static T* Register()
	{
		T* module = new T();

		for (auto name : module->GetNames())
		{
			s_Instance->m_ModuleMap[name] = module;
		}

		s_Instance->m_Modules.push_back(module);

		return module;
	}

	template<typename T>
	static T* Get()
	{
		auto it = s_Instance->m_ModuleMap.find(T::StaticName());
		if (it == s_Instance->m_ModuleMap.end())
		{
			return nullptr;
		}

		return dynamic_cast<T*>(it->second);
	}

};
