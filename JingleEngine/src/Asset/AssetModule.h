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

	//! TODO: Config files read seperately so the root type can be used

	template <typename T>
	static Ref<T> Get(std::string path, JingleScript::Type* type = nullptr);

	template <typename T>
	static Ref<T> Get(AssetID id, JingleScript::Type* type = nullptr);

	template <typename T>
	static Ref<T> Get(AssetIDv id, JingleScript::Type* type = nullptr);

private:
	static void Unload(AssetID id);

	static AssetIDv ConvertPath(std::string path);

};

template <typename T>
static Ref<T> AssetModule::Get(std::string path, JingleScript::Type* type)
{
	return Get<T>(AssetID(path), type);
}

template <typename T>
static Ref<T> AssetModule::Get(AssetID id, JingleScript::Type* type)
{
	if (type == nullptr)
	{
		type = T::StaticType();
	}

	auto it = s_Instance->m_Assets.find(id.GetValue());
	if (it == s_Instance->m_Assets.end())
	{
		JS_INFO("Create Asset: %s", T::StaticName().c_str());

		T* asset = type->New<T>();
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
static Ref<T> AssetModule::Get(AssetIDv id, JingleScript::Type* type)
{
	if (type == nullptr)
	{
		type = T::StaticType();
	}

	auto it = s_Instance->m_Assets.find(id);
	if (it == s_Instance->m_Assets.end())
	{
		JS_INFO("Create Asset: %s", T::StaticName().c_str());

		T* asset = type->New<T>();
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
