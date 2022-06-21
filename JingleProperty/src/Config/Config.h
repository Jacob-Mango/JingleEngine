#pragma once

#include "Core.h"

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

class Config : public Countable
{
	typedef Countable Super;

	friend ConfigArray;
	friend ConfigSection;
	friend ConfigValue;

protected:
	std::string m_Name;

	Config* m_Parent;

public:
	virtual void Add(Config* other) {}

	virtual size_t Count() const { return 0; }

	virtual std::string GetValue() const { return ""; }
	virtual std::string GetValue(std::string name) const { return Get(name)->GetValue(); }
	virtual std::string GetValue(int index) const { return Get(index)->GetValue(); }

	virtual Config* Get(std::string name) const { return nullptr; }
	virtual Config* Get(int index) const { return nullptr; }

	virtual std::string GetName() const { return m_Name; }
	virtual Config* GetBase() const { return nullptr; }
	virtual Config* GetParent() const { return m_Parent; }

	virtual bool Deserialize(JingleScript::Lexer* lexer);
	virtual void Serialize(std::ostringstream& output, std::string prefix = "") const;
	
	virtual std::string ToString() const override;

public:
	static Config* Load(std::string file);

};
