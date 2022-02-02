#pragma once

#include <unordered_map>
#include <string>

#include "../Core/Logging.h"

#include "Asset.h"
#include "GUID.h"

class AssetManager
{
	friend class Asset;
	friend class GUID;

private:
	static AssetManager* s_Instance;

	std::unordered_map<GUIDv, Ref<Asset>> m_Assets;

public:
	static void Init();
	static void Destroy();

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
static Ref<T> AssetManager::Get(std::string path)
{
	return Get<T>(GUID(path));
}

template <typename T>
static Ref<T> AssetManager::Get(GUID guid)
{
	auto it = s_Instance->m_Assets.find(guid.GetValue());
	if (it == s_Instance->m_Assets.end())
	{
		s_Instance->m_Assets.insert({ guid.GetValue(), new T(guid) });
	}

	Ref<T> result = dynamic_cast<T*>(s_Instance->m_Assets[guid.GetValue()].Get());

	if (!result->IsLoaded() && result->OnLoad())
	{
		result->m_IsLoaded = true;
	}

	return result;
}

template <typename T>
static Ref<T> AssetManager::Get(GUIDv guid)
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

