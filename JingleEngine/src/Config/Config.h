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

class Config
{
	typedef Countable Super;

	friend Config;
	friend ConfigArray;
	friend ConfigSection;
	friend ConfigValue;

protected:
	std::string m_CType;
	std::string m_Name;

	Config* m_Parent;

protected:
	Config();
	Config(Config&) = delete;
	Config(Config&&) = delete;
	~Config();

public:
	virtual void Add(Config* other) {}

	virtual size_t Count() const { return 0; }

	virtual std::string GetValue() const { return ""; }
	virtual std::string* GetValuePtr() { return nullptr; }
	virtual std::string GetValue(std::string name) const { return Get(name)->GetValue(); }
	virtual std::string GetValue(int index) const { return Get(index)->GetValue(); }

	virtual Config* Get(std::string name) const { return nullptr; }
	virtual Config* Get(int index) const { return nullptr; }

	virtual std::string GetName() const { return m_Name; }
	virtual std::string GetType() const { return m_CType; }
	virtual Config* GetBase() const { return nullptr; }
	virtual Config* GetParent() const { return m_Parent; }

	virtual bool Deserialize(JingleScript::Lexer* lexer);
	virtual bool Serialize(std::stringstream& output, std::string prefix = "") const;

protected:
	bool DeserializeTypeAndName(JingleScript::Lexer* lexer, std::pair<std::string, std::string>& result, bool checkColon = true);
	std::string SerializeTypeAndName() const;

};
