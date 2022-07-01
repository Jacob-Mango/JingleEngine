#pragma once

#include "Core/Core.h"

#include "Asset/Asset.h"

#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class ConfigArray;
class ConfigAsset;
class ConfigSection;
class ConfigValue;

namespace JingleScript
{
	class Lexer;
}

struct ConfigTypeInfo
{
	std::string m_Type;
	std::string m_Name;
	bool m_DirectlyLinked = true;
};

class Config : public JingleScript::ManagedObject
{
	DEFINE_CLASS(Config, JingleScript::ManagedObject);

	friend Config;
	friend ConfigArray;
	friend ConfigSection;
	friend ConfigValue;

protected:
	ConfigTypeInfo m_TypeInfo;

	Config* m_Parent;

public:
	Config();
	~Config();

public:
	virtual Config* Insert(Config* other) { return nullptr; }
	virtual Config* Remove(Config* other) { return nullptr; }

	virtual size_t Count() const { return 0; }

	virtual void SetValue(const std::string& value) { }
	void SetValue(const std::string& name, const std::string& value) { Config* cfg = Get(name); if (cfg) { cfg->SetValue(value); } }
	void SetValue(int index, const std::string& value) { Config* cfg = Get(index); if (cfg) { cfg->SetValue(value); } }

	virtual std::string GetValue() const { return ""; }
	std::string GetValue(const std::string& name) const { Config* cfg = Get(name); return cfg ? cfg->GetValue() : ""; }
	std::string GetValue(int index) const { Config* cfg = Get(index); return cfg ? cfg->GetValue() : ""; }

	virtual std::string* GetValuePtr() { return nullptr; }

	virtual Config* Get(std::string name) const { return nullptr; }
	virtual Config* Get(int index) const { return nullptr; }

	std::string GetName() const;
	void SetName(std::string name);

	std::string GetLinkedType() const;
	
	void SetLinkedType(std::string type);
	void SetLinkedType(std::string type, bool directly);

	std::string GetTypeAndName() const;
	bool IsLinkedDirectly() const;

	virtual ConfigAsset* GetBase() const { return nullptr; }
	virtual Config* GetParent() const { return m_Parent; }

	Config* CreateValue(const std::string& name, const std::string& value, JingleScript::Type* type = nullptr);
	Config* CreateSection(const std::string& name, JingleScript::Type* type = nullptr);
	Config* CreateArray(const std::string& name, JingleScript::Type* type = nullptr);

	virtual bool Deserialize(JingleScript::Lexer* lexer, Config* parent);
	virtual bool Serialize(std::stringstream& output, std::string prefix = "") const;

protected:
	bool DeserializeTypeAndName(JingleScript::Lexer* lexer, ConfigTypeInfo& result);
	std::string SerializeTypeAndName() const;

};
