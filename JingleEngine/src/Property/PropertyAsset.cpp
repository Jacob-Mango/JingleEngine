#include "Property/PropertyAsset.h"

using namespace JingleScript;

BEGIN_CLASS_LINK(PropertyAsset)
	LINK_CONSTRUCTOR(Type*, Property*, uint64_t);
END_CLASS_LINK()

PropertyAsset::PropertyAsset(Type* type, Property* property, uint64_t offset)
	: PropertyBase(type, property), m_Offset(offset)
{
	JS_TRACE(Tracers::Property);

}

PropertyAsset::~PropertyAsset()
{
	JS_TRACE(Tracers::Property);

}

bool PropertyAsset::OnDeserialize(Config* cfg)
{
	JS_TRACE(Tracers::Property);

	if (cfg)
	{
		m_ID = *cfg->GetValuePtr();
	}

	return true;
}

bool PropertyAsset::OnSerialize(Config* cfg)
{
	JS_TRACE(Tracers::Property);

	cfg->SetValue(m_ID.GetPath());
	return true;
}

bool PropertyAsset::OnReadObject(Object* instance)
{
	JS_TRACE(Tracers::Property);
	JS_TINFO("Instance: {}", PointerToString(instance));

	Asset* asset = (Asset*)((char*)instance + m_Offset);
	if (asset != nullptr)
	{
		m_ID = asset->GetAssetID();
	}

	return true;
}

bool PropertyAsset::OnWriteObject(Object* instance)
{
	JS_TRACE(Tracers::Property);
	JS_TINFO("Instance: {}", PointerToString(instance));

	Asset* asset = AssetModule::Get<Asset>(m_ID, m_PropertyType);
	*(void**)((char*)instance + m_Offset) = asset;
	
	return true;
}

Object* PropertyAsset::GetReadInstance(Object* instance)
{
	JS_TRACE(Tracers::Property);

	return instance;
}

Object* PropertyAsset::GetWriteInstance(Object* instance)
{
	JS_TRACE(Tracers::Property);

	return instance;
}
