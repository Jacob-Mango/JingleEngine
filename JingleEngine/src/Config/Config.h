#pragma once

#include "Core/Core.h"

#include "Asset/Asset.h"

#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class ConfigArray;
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

class Config : public JingleScript::Object
{
	DEFINE_CLASS(Config, JingleScript::Object);

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
	virtual void Add(Config* other) {}

	virtual size_t Count() const { return 0; }

	virtual std::string GetValue() const { return ""; }
	virtual void SetValue(std::string) { }
	virtual std::string* GetValuePtr() { return nullptr; }
	virtual std::string GetValue(std::string name) const { return Get(name)->GetValue(); }
	virtual std::string GetValue(int index) const { return Get(index)->GetValue(); }

	virtual Config* Get(std::string name) const { return nullptr; }
	virtual Config* Get(int index) const { return nullptr; }

	std::string GetName() const;
	std::string GetLinkedType() const;
	std::string GetTypeAndName() const;
	bool IsLinkedDirectly() const;

	virtual Config* GetBase() const { return nullptr; }
	virtual Config* GetParent() const { return m_Parent; }

	virtual bool Deserialize(JingleScript::Lexer* lexer, Config* parent);
	virtual bool Serialize(std::stringstream& output, std::string prefix = "") const;

protected:
	bool DeserializeTypeAndName(JingleScript::Lexer* lexer, ConfigTypeInfo& result);
	std::string SerializeTypeAndName() const;

};
