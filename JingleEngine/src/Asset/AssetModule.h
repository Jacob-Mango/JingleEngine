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

	Asset* Get(AssetID id, JingleScript::Type* type);

	template <typename T>
	static T* Get(std::string path, JingleScript::Type* type = nullptr);

	template <typename T>
	static T* Get(AssetID id, JingleScript::Type* type = nullptr);

	template <typename T>
	static T* Get(AssetIDv id, JingleScript::Type* type = nullptr);

private:
	static void Unload(AssetID id);

	static AssetIDv ConvertPath(std::string path);

};

template <typename T>
static T* AssetModule::Get(std::string path, JingleScript::Type* type)
{
	return Get<T>(AssetID(path), type);
}

template <typename T>
static T* AssetModule::Get(AssetID id, JingleScript::Type* type)
{
	if (type == nullptr)
	{
		type = T::StaticType();
	}

	Asset* asset = s_Instance->Get(id, type);
	if (!asset)
	{
		return nullptr;
	}

	return static_cast<T*>(asset);
}

template <typename T>
static T* AssetModule::Get(AssetIDv id, JingleScript::Type* type)
{
	if (type == nullptr)
	{
		type = T::StaticType();
	}

	Asset* asset = s_Instance->Get({ id }, type);
	if (!asset)
	{
		return nullptr;
	}

	return static_cast<T*>(asset);
}
