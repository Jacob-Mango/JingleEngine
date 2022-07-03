#pragma once

#include "Core/Core.h"

#include "Asset/Asset.h"

#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Config/ConfigIterator.h"

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
};

class Config : public JingleScript::ManagedObject
{
	DEFINE_CLASS(Config, JingleScript::ManagedObject);

	friend Config;
	friend ConfigSection;
	friend ConfigValue;

	friend ConfigIterator;

protected:
	ConfigTypeInfo m_TypeInfo;

	Config* m_Parent;

public:
	Config();
	~Config();

public:
	Config* CreateValue(const std::string& name, const std::string& value, JingleScript::Type* type = nullptr);
	Config* CreateSection(const std::string& name, JingleScript::Type* type = nullptr);
	Config* CreateArray(const std::string& name, JingleScript::Type* type = nullptr);

public:
	virtual ConfigIterator begin() { return end(); }
	virtual ConfigIterator end() { return ConfigIterator(); }

public:
	void SetName(std::string name);
	std::string GetName() const;

	void SetLinkedType(std::string type);
	std::string GetLinkedType() const;

	virtual std::string ToString() const override;
	virtual void Debug() {}

	std::string GetTypeAndName() const;

public:
	virtual void SetArray(bool isArray) {}
	virtual bool IsArray() const { return false; }

	virtual void UpdateBase() {}

	virtual ConfigSection* GetBase() const { return nullptr; }
	virtual ConfigAsset* GetBaseAsset() const { return nullptr; }
	
	virtual Config* GetParent() const { return m_Parent; }

	virtual Config* Set(Config* other) { return nullptr; }
	virtual Config* Remove(Config* other) { return nullptr; }

	virtual Config* Get(std::string name) const { return nullptr; }

	virtual bool Optimize(Config* source, bool isBaseCheck) { return true; }

public:
	virtual void SetValue(const std::string& value) { }
	virtual std::string GetValue() const { return ""; }
	virtual std::string* GetValuePtr() { return nullptr; }

	void SetValue(const std::string& name, const std::string& value) { Config* cfg = Get(name); if (cfg) { cfg->SetValue(value); } }
	std::string GetValue(const std::string& name) const { Config* cfg = Get(name); return cfg ? cfg->GetValue() : ""; }

public:
	virtual bool Deserialize(JingleScript::Lexer* lexer, Config* parent);
	virtual bool Serialize(std::stringstream& output, std::string prefix = "") const;

protected:
	bool DeserializeTypeAndName(JingleScript::Lexer* lexer, ConfigTypeInfo& result);
	std::string SerializeTypeAndName() const;

};
