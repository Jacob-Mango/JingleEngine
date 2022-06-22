#pragma once

#include "Core/Core.h"

#include <unordered_map>
#include <string>

#include "Core/Logging.h"
#include "Core/Module.h"

#include "Asset/Asset.h"
#include "Asset/AssetID.h"

class Asset;
class AssetID;

class AssetModule : public Module
{
	DEFINE_MODULE(AssetModule, Module);

	friend Asset;
	friend AssetID;

private:
	static AssetModule* s_Instance;

	std::unordered_map<AssetIDv, Ref<Asset>> m_Assets;
	bool m_IsDestroying = false;

public:
	virtual void OnPreInitialize() override;
	virtual void OnInitialize() override;

	virtual void OnDestroy() override;

	template <typename T>
	static Ref<T> Get(std::string path);

	template <typename T>
	static Ref<T> Get(AssetID id);

	template <typename T>
	static Ref<T> Get(AssetIDv id);

private:
	static void Unload(AssetID id);

	static AssetIDv ConvertPath(std::string path);

};

template <typename T>
static Ref<T> AssetModule::Get(std::string path)
{
	return Get<T>(AssetID(path));
}

template <typename T>
static Ref<T> AssetModule::Get(AssetID id)
{
	auto it = s_Instance->m_Assets.find(id.GetValue());
	if (it == s_Instance->m_Assets.end())
	{
		JS_INFO("Create Asset: %s", T::StaticName().c_str());

		T* asset = JingleScript::NewObject<T>(T::StaticName());
		asset->m_AssetID = id;
		s_Instance->m_Assets.insert({ id.GetValue(), asset });
	}

	Ref<T> result = dynamic_cast<T*>(s_Instance->m_Assets[id.GetValue()].Get());

	if (!result->IsLoaded() && result->OnLoad())
	{
		result->m_IsLoaded = true;
	}

	return result;
}

template <typename T>
static Ref<T> AssetModule::Get(AssetIDv id)
{
	auto it = s_Instance->m_Assets.find(id);
	if (it == s_Instance->m_Assets.end())
	{
		JS_INFO("Create Asset: %s", T::StaticName().c_str());

		T* asset = JingleScript::NewObject<T>(T::StaticName());
		asset->m_AssetID = { id };
		s_Instance->m_Assets.insert({ id, asset });
	}

	Ref<T> result = dynamic_cast<T*>(s_Instance->m_Assets[id].Get());

	if (!result->IsLoaded() && result->OnLoad())
	{
		result->m_IsLoaded = true;
	}

	return result;
}
