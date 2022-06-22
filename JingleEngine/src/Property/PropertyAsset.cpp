#include "Property/PropertyAsset.h"

using namespace JingleScript;

PropertyAsset::PropertyAsset(Type* type, Property* property, uint64_t offset)
	: PropertyBase(type, property), m_Offset(offset)
{

}

PropertyAsset::~PropertyAsset()
{

}

bool PropertyAsset::OnDeserialize(Config* cfg)
{
	m_ID = *cfg->GetValuePtr();
	return true;
}

bool PropertyAsset::OnSerialize(Config* cfg)
{
	cfg->SetValue(m_ID.GetPath());
	return true;
}

bool PropertyAsset::OnReadObject(Object* instance)
{
	Asset* asset = (Asset*)((char*)instance + m_Offset);
	if (asset != nullptr)
	{
		m_ID = asset->GetAssetID();
	}

	return true;
}

bool PropertyAsset::OnWriteObject(Object* instance)
{
	Asset* asset = AssetModule::Get<Asset>(m_ID, m_Type);
	*(void**)((char*)instance + m_Offset) = asset;
	
	return true;
}

Object* PropertyAsset::GetReadInstance(Object* instance)
{
	return instance;
}

Object* PropertyAsset::GetWriteInstance(Object* instance)
{
	return instance;
}
