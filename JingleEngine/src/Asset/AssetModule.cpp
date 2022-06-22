#include "Asset/AssetModule.h"

#include <iostream>

#include "Core/Logging.h"
#include "Core/ModuleManager.h"

AssetModule *AssetModule::s_Instance;

BEGIN_MODULE_LINK(AssetModule);
END_MODULE_LINK();

void AssetModule::OnPreInitialize()
{
	s_Instance = this;
}

void AssetModule::OnInitialize()
{
	
}

void AssetModule::OnDestroy()
{
	s_Instance->m_Assets.erase(s_Instance->m_Assets.begin(), s_Instance->m_Assets.end());
}

AssetIDv CRCByte(AssetIDv input, AssetIDv divisor)
{
	for (int k = 16; k > 0; k--)
	{
		input = input & 1 ? (input >> 1) ^ divisor : input >> 1;
	}

	return input;
}

AssetIDv AssetModule::ConvertPath(std::string path)
{
	int i;
	AssetIDv crc = 0xFFFFFFFFFFFFFFFF;

	for (i = 0; i < path.length(); i++)
	{
		char c = path[i];
		if (c > 96)
		{
			c -= 32;
		}

		crc = CRCByte((crc ^ c) & 0xff, 0xEDB88320EDB88320) ^ (crc >> 8);
	}

	return crc ^ 0xFFFFFFFFFFFFFFFF;
}

void AssetModule::Unload(AssetID id)
{
	auto it = s_Instance->m_Assets.find(id);
	if (it == s_Instance->m_Assets.end())
	{
		return;
	}

	s_Instance->m_Assets.erase(it);
}
