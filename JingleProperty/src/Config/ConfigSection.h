#pragma once

#include "Config/Config.h"

class ConfigSection : public Config
{
	typedef Config Super;

public:
	std::map<std::string, Config*> m_Entries;

	Config* m_Base;

public:
	virtual void Add(Config* other) override;

	virtual size_t Count() const { return m_Entries.size(); }

	virtual Config* Get(std::string name) const override;
	virtual Config* Get(int index) const override;

	virtual Config* GetBase() const override;

	virtual bool Deserialize(JingleScript::Lexer* lexer) override;
	virtual void Serialize(std::ostringstream& output, std::string prefix = "") const override;

	virtual std::string ToString() const override;

};
