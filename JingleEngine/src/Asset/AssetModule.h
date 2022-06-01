#pragma once

#include "Core/Core.h"

#include <unordered_map>
#include <string>

#include "Core/Logging.h"
#include "Core/Module.h"

#include "Asset/Asset.h"
#include "Asset/GUID.h"

class AssetModule : public Module
{
	DEFINE_MODULE(AssetModule, Module);

	friend class Asset;
	friend class GUID;

private:
	static AssetModule* s_Instance;

	std::unordered_map<GUIDv, Ref<Asset>> m_Assets;

public:
	AssetModule() {}

	virtual void OnPreInitialize() override;
	virtual void OnInitialize() override;

	virtual void OnDestroy() override;

	template <typename T>
	static Ref<T> Get(std::string path);

	template <typename T>
	static Ref<T> Get(GUID guid);

	template <typename T>
	static Ref<T> Get(GUIDv guid);

private:
	static void Unload(GUID guid);

	static GUIDv ConvertPath(std::string path);
};

template <typename T>
static Ref<T> AssetModule::Get(std::string path)
{
	return Get<T>(GUID(path));
}

template <typename T>
static Ref<T> AssetModule::Get(GUID guid)
{
	auto it = s_Instance->m_Assets.find(guid.GetValue());
	if (it == s_Instance->m_Assets.end())
	{
	JS_INFO("Create asset: %s", T::StaticName().c_str());
		T* asset = JingleScript::NewObject<T>(T::StaticName());
		asset->m_GUID = guid;
		s_Instance->m_Assets.insert({ guid.GetValue(), asset });
	}

	Ref<T> result = dynamic_cast<T*>(s_Instance->m_Assets[guid.GetValue()].Get());

	if (!result->IsLoaded() && result->OnLoad())
	{
		result->m_IsLoaded = true;
	}

	return result;
}

template <typename T>
static Ref<T> AssetModule::Get(GUIDv guid)
{
	auto it = s_Instance->m_Assets.find(guid);
	if (it == s_Instance->m_Assets.end())
	{
		s_Instance->m_Assets.insert({ guid, new T(new GUID(guid)) });
	}

	Ref<T> result = dynamic_cast<T*>(s_Instance->m_Assets[guid].Get());

	if (!result->IsLoaded() && result->OnLoad())
	{
		result->m_IsLoaded = true;
	}

	return result;
}
