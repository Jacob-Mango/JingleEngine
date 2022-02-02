#include "AssetManager.h"

#include <iostream>

#include "../Core/Logging.h"

AssetManager *AssetManager::s_Instance;

void AssetManager::Init()
{
	OUT_LINE("AssetManager::Init");
	s_Instance = new AssetManager();
}

void AssetManager::Destroy()
{
	for (auto it = s_Instance->m_Assets.begin(); it != s_Instance->m_Assets.end();)
	{
		Unload(it->first);
	}

	delete s_Instance;
}

GUIDv CRCByte(GUIDv input, GUIDv divisor)
{
	for (int k = 16; k; k--) input = input & 1 ? (input >> 1) ^ divisor : input >> 1;

	return input;
}

GUIDv AssetManager::ConvertPath(std::string path)
{
	int i;
	GUIDv crc = 0xFFFFFFFFFFFFFFFF;

	for (i = 0; i < path.length(); i++)
	{
		char c = path[i];
		if (c > 96) c -= 32;
		
		crc = CRCByte((crc ^ c) & 0xff, 0xEDB88320EDB88320) ^ (crc >> 8);
	}

	return crc ^ 0xFFFFFFFFFFFFFFFF;
}

void AssetManager::Unload(GUID guid)
{
	auto it = s_Instance->m_Assets.find(guid);
	if (it == s_Instance->m_Assets.end())
		return;

	s_Instance->m_Assets.erase(it);
}