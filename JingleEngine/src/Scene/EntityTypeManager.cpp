#include "EntityTypeManager.h"

std::map<std::string, Ref<EntityType>> EntityTypeManager::s_BaseEntityTypes;
std::map<std::string, Ref<EntityType>> EntityTypeManager::s_EntityTypes;

void EntityTypeManager::Load(Config& config)
{
	EntityType* type = nullptr;
	Config* base = &config;

	while (base != nullptr && type == nullptr)
	{
		auto it = s_BaseEntityTypes.find(base->m_Name);
		if (it == s_BaseEntityTypes.end())
		{
			base = base->GetBase();
			continue;
		}

		type = it->second;
	}

	if (type == nullptr)
		return;

	type = type->CreateType();
	type->Load(config);

	s_EntityTypes[config.m_Name] = type;
}

void EntityTypeManager::_Register(EntityType* type)
{
	s_BaseEntityTypes.insert({ type->GetClassName(), type });
}

Ref<EntityType> EntityTypeManager::_Get(std::string type)
{
	auto it = s_EntityTypes.find(type);
	if (it == s_EntityTypes.end())
		return nullptr;

	return it->second;
}
