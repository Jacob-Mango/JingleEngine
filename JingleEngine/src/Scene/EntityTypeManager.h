#pragma once

#include "Scene/Entity.h"
#include "Core/Config.h"

class EntityTypeManager
{
private:
	static std::map<std::string, Ref<EntityType>> s_BaseEntityTypes;
	static std::map<std::string, Ref<EntityType>> s_EntityTypes;

public:
	static void Load(Config& config);

private:
	static void _Register(EntityType* type);
	static Ref<EntityType> _Get(std::string type);

public:
	template<typename T>
	static void Register()
	{
		_Register(new T());
	}

	template<typename T>
	static Ref<T> Get()
	{
		return _Get(T::StaticName()).As<T>();
	}

	template<typename T>
	static Ref<T> Get(std::string type)
	{
		return _Get(type).As<T>();
	}
	
};
