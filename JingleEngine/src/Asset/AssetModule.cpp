#include "Asset/AssetModule.h"

#include "Config/ConfigAsset.h"

#include "Core/ModuleManager.h"

#include <iostream>

using namespace JingleScript;
AssetModule *AssetModule::s_Instance;

BEGIN_MODULE_LINK(AssetModule);
END_MODULE_LINK();

void AssetModule::OnPreInitialize()
{
	JS_TRACE(Tracers::Asset);

	s_Instance = this;
}

void AssetModule::OnInitialize()
{
	JS_TRACE(Tracers::Asset);
	
}

void AssetModule::OnDestroy()
{
	JS_TRACE(Tracers::Asset);

	m_Assets.erase(m_Assets.begin(), m_Assets.end());
}

Asset* AssetModule::Get(AssetID id, Type* type)
{
	JS_TRACE(Tracers::Asset);

	Asset* asset = nullptr;

	JS_TINFO("Searching for asset {:x}:{} of type {}", id.GetValue(), id.GetPath(), type->Name());

	auto it = s_Instance->m_Assets.find(id.GetValue());
	if (it == s_Instance->m_Assets.end())
	{
		bool isCfg = type->IsInherited(ConfigAsset::StaticType());
		JS_TINFO("Is Config: {}", isCfg);
		if (isCfg)
		{
			std::string path = id.GetPath();
			if (path.empty())
			{
				JS_ERROR("Failed to load asset, no path provided.");
				return nullptr;
			}

			Ref<Lexer> lexer = Lexer::ParseFile(path);
			if (!lexer->HasNext())
			{
				JS_ERROR("Failed to load asset '{}', empty file.", path);
				return nullptr;
			}

			ConfigSection* cfg = NewObject<ConfigSection>("ConfigSection");
			if (!cfg->Deserialize(lexer, nullptr))
			{
				JS_ERROR("Failed to load asset '{}', deserialization failed.", path);
				return nullptr;
			}

			if (cfg->IsLinkedDirectly() && !cfg->GetLinkedType().empty())
			{
				Type* cfgType = TypeManager::Get(cfg->GetLinkedType());
				if (!cfgType)
				{
					JS_ERROR("Failed to load asset '{}', invalid root type.", path);
					return nullptr;
				}

				if (!cfgType->IsInherited(type))
				{
					JS_ERROR("Failed to load asset '{}', root type '{}' not of same ancestor '{}'.", path, cfgType->Name(), type->Name());
					return nullptr;
				}

				type = cfgType;
			}

			ConfigAsset* cfgAsset = type->New<ConfigAsset>();
			cfgAsset->m_Config = cfg;

			if (!cfgAsset->m_Properties->OnDeserialize(cfg))
			{
				JS_ERROR("Failed to load asset '{}', couldn't deserialize data.", path);
				return nullptr;
			}

			if (cfg->IsLinkedDirectly() && !cfgAsset->WriteToObject(cfgAsset))
			{
				JS_ERROR("Failed to load asset '{}', couldn't write data.", path);
				return nullptr;
			}

			asset = cfgAsset;
		}
		else
		{
			asset = type->New<Asset>();
		}

		asset->m_AssetID = id;
		if (!asset->OnLoad())
		{
			return nullptr;
		}

		m_Assets.insert({ id.GetValue(), asset });
	}
	else
	{
		asset = m_Assets[id.GetValue()];
	}

	return asset;
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
	//JS_TRACE(Tracers::Asset);

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

	crc = crc ^ 0xFFFFFFFFFFFFFFFF;

	//JS_TINFO("\"{:x}:{}\"", crc, path);

	return crc;
}

void AssetModule::Unload(AssetID id)
{
	JS_TRACE(Tracers::Asset);

	auto it = s_Instance->m_Assets.find(id);
	if (it == s_Instance->m_Assets.end())
	{
		return;
	}

	s_Instance->m_Assets.erase(it);
}
