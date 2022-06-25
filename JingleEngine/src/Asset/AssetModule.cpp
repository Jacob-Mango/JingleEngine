#include "Asset/AssetModule.h"

#include "Config/ConfigAsset.h"

#include "Core/Logging.h"
#include "Core/ModuleManager.h"

#include <iostream>

using namespace JingleScript;
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
	m_Assets.erase(m_Assets.begin(), m_Assets.end());
}

Asset* AssetModule::Get(AssetID id, Type* type)
{
	Asset* asset = nullptr;

	auto it = s_Instance->m_Assets.find(id.GetValue());
	if (it == s_Instance->m_Assets.end())
	{
		bool isCfg = type->IsInherited(ConfigAsset::StaticType());
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
				JS_ERROR("Failed to load asset '%s', empty file.", path.c_str());
				return nullptr;
			}

			ConfigSection* cfg = NewObject<ConfigSection>("ConfigSection");
			if (!cfg->Deserialize(lexer, nullptr))
			{
				JS_ERROR("Failed to load asset '%s', deserialization failed.", path.c_str());
				return nullptr;
			}

			if (cfg->IsLinkedDirectly() && !cfg->GetLinkedType().empty())
			{
				Type* cfgType = TypeManager::Get(cfg->GetLinkedType());
				if (!cfgType)
				{
					JS_ERROR("Failed to load asset '%s', invalid root type.", path.c_str());
					return nullptr;
				}

				if (!cfgType->IsInherited(type))
				{
					JS_ERROR("Failed to load asset '%s', root type '%s' not of same ancestor '%s'.", path.c_str(), cfgType->Name().c_str(), type->Name().c_str());
					return nullptr;
				}

				type = cfgType;
			}

			ConfigAsset* cfgAsset = type->New<ConfigAsset>();
			cfgAsset->m_Config = cfg;

			if (!cfgAsset->m_Properties->OnDeserialize(cfg))
			{
				JS_ERROR("Failed to load asset '%s', couldn't deserialize data.", path.c_str());
				return nullptr;
			}

			if (cfg->IsLinkedDirectly() && !cfgAsset->WriteToObject(cfg))
			{
				JS_ERROR("Failed to load asset '%s', couldn't write data.", path.c_str());
				return nullptr;
			}

			asset = cfgAsset;
		}
		else
		{
			asset = type->New<Asset>();
		}

		//std::cout << "Loading path=" << id.GetPath() << " type=" << type->Name() << " as " << (isCfg ? "Config" : "Asset") << std::endl;

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
