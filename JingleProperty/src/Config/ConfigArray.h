#pragma once

#include "Config/Config.h"

class ConfigArray : public Config
{
	typedef Config Super;

protected:
	std::vector<Config*> m_Entries;

public:
	virtual void Add(Config* other) override;

	virtual size_t Count() const { return m_Entries.size(); }
	virtual Config* Get(int index) const override;

	virtual bool Deserialize(JingleScript::Lexer* lexer) override;
	virtual void Serialize(std::stringstream& output, std::string prefix = "") const override;

	virtual std::string ToString() const override;

};
