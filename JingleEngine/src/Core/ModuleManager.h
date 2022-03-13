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
	std::unordered_map<std::string, Module*> m_ModuleMap;
	std::vector<Module*> m_Modules;
	ModuleIterator m_Iterator;

	ModuleManager() {}
	ModuleManager(const ModuleManager&&) = delete;
	ModuleManager(const ModuleManager&) = delete;

public:
	static void Initialize();

	static ModuleManager& Get();

	static ModuleIterator& Iterator();

	static void On(std::function<void(Module*)> func);

	template<typename T>
	static T* Get()
	{
		auto it = Get().m_ModuleMap.find(T::StaticName());
		if (it == Get().m_ModuleMap.end())
		{
			return nullptr;
		}

		return dynamic_cast<T*>(it->second);
	}

};
