#pragma once

#include "Core/Core.h"

#include <unordered_map>
#include <string>

#include "Core/Logging.h"
#include "Core/Module.h"

#include "Asset/Asset.h"
#include "Asset/AssetID.h"

class AssetModule : public Module
{
	DEFINE_MODULE(AssetModule, Module);

	friend class Asset;
	friend class AssetID;

private:
	static AssetModule* s_Instance;

	std::unordered_map<AssetIDv, Ref<Asset>> m_Assets;

public:
	virtual void OnPreInitialize() override;
	virtual void OnInitialize() override;

	virtual void OnDestroy() override;

	template <typename T>
	static Ref<T> Get(std::string path);

	template <typename T>
	static Ref<T> Get(AssetID AssetID);

	template <typename T>
	static Ref<T> Get(AssetIDv AssetID);

private:
	static void Unload(AssetID AssetID);

	static AssetIDv ConvertPath(std::string path);

};

template <typename T>
static Ref<T> AssetModule::Get(std::string path)
{
	return Get<T>(AssetID(path));
}

template <typename T>
static Ref<T> AssetModule::Get(AssetID AssetID)
{
	auto it = s_Instance->m_Assets.find(AssetID.GetValue());
	if (it == s_Instance->m_Assets.end())
	{
	JS_INFO("Create asset: %s", T::StaticName().c_str());
		T* asset = JingleScript::NewObject<T>(T::StaticName());
		asset->m_AssetID = AssetID;
		s_Instance->m_Assets.insert({ AssetID.GetValue(), asset });
	}

	Ref<T> result = dynamic_cast<T*>(s_Instance->m_Assets[AssetID.GetValue()].Get());

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
		s_Instance->m_Assets.insert({ id, new T(new AssetID(id)) });
	}

	Ref<T> result = dynamic_cast<T*>(s_Instance->m_Assets[id].Get());

	if (!result->IsLoaded() && result->OnLoad())
	{
		result->m_IsLoaded = true;
	}

	return result;
}
