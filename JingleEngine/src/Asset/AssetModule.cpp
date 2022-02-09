#include "AssetModule.h"

#include <iostream>

#include "Core/Logging.h"

AssetModule *AssetModule::s_Instance;

void AssetModule::OnCreate()
{
	s_Instance = this;
}

void AssetModule::OnDestroy()
{
	for (auto it = s_Instance->m_Assets.begin(); it != s_Instance->m_Assets.end();)
	{
		Unload(it->first);
	}
}

GUIDv CRCByte(GUIDv input, GUIDv divisor)
{
	for (int k = 16; k; k--) input = input & 1 ? (input >> 1) ^ divisor : input >> 1;

	return input;
}

GUIDv AssetModule::ConvertPath(std::string path)
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

void AssetModule::Unload(GUID guid)
{
	auto it = s_Instance->m_Assets.find(guid);
	if (it == s_Instance->m_Assets.end())
		return;

	s_Instance->m_Assets.erase(it);
}
