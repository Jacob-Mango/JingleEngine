#include "EntityTypeManager.h"

#include <JingleScript.h>

std::map<std::string, EntityType*> EntityTypeManager::s_EntityTypes;

EntityType* EntityTypeManager::Load(Config& config)
{
	EntityType* type = JingleScript::NewObject<EntityType>(config.m_Name + "Type");
	type->Load(config);

	s_EntityTypes[config.m_Name] = type;
	return type;
}

EntityType* EntityTypeManager::_Get(std::string type)
{
	auto it = s_EntityTypes.find(type);
	if (it == s_EntityTypes.end())
		return nullptr;

	return it->second;
}
