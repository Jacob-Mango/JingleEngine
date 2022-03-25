#pragma once

#include "Core/Core.h"

#include "Scene/Entity.h"
#include "Core/Config.h"

class EntityTypeManager
{
private:
	static std::map<std::string, EntityType*> s_EntityTypes;

public:
	static EntityType* Load(Config& config);

private:
	static EntityType* _Get(std::string type);

public:
	template<typename T>
	static T* Get()
	{
		return static_cast<T*>(_Get(T::StaticName()));
	}

	template<typename T>
	static T* Get(std::string type)
	{
		return static_cast<T*>(_Get(type));
	}
	
};
