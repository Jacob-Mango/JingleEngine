#pragma once

#include "Core.h"

#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

class Config;

extern Config* g_NullConfig;

class Config : public Countable
{
	typedef Countable Super;

public:
	std::string m_Name;

	Config* m_Parent;

public:
	union {
		int Int;
		int Count;
	};

	std::string String;
	float Float;

	glm::vec3 Vec3()
	{
		glm::vec3 vec;

		std::istringstream stream(String);

		stream >> vec.x;
		stream >> vec.y;
		stream >> vec.z;

		return vec;
	}

public:
	Config();
	virtual ~Config();

	virtual void Add(Config* other);

	virtual Config* Get(std::string name);
	virtual Config* Get(int index);

	virtual std::string GetName();
	virtual Config* GetBase();
	virtual Config* GetParent();

	virtual bool IsNull();

	Config& operator[](std::string name);
	Config& operator[](int index);

	static Config* Load(std::string file);

	virtual void ToString(std::ostringstream& output, std::string prefix = "");

	virtual std::string ToString() const override;

public:
	virtual bool Load(std::ifstream& stream);
};

class ConfigNull : public Config
{
	typedef Config Super;

public:
	virtual bool IsNull() override;

	virtual void ToString(std::ostringstream& output, std::string prefix = "") override;

	virtual std::string ToString() const override;

};

class ConfigValue : public Config
{
	typedef Config Super;

public:
	virtual void ToString(std::ostringstream& output, std::string prefix = "") override;

	virtual std::string ToString() const override;

};

class ConfigArray : public Config
{
	typedef Config Super;

	std::vector<Config*> m_Entries;

public:
	virtual void Add(Config* other) override;

	virtual Config* Get(int index) override;

	virtual void ToString(std::ostringstream& output, std::string prefix = "") override;

	virtual std::string ToString() const override;

public:
	virtual bool Load(std::ifstream& stream) override;
};

class ConfigSection : public Config
{
	typedef Config Super;

public:
	std::map<std::string, Config*> m_Entries;

	Config* m_Base;

public:
	virtual void Add(Config* other) override;

	virtual Config* Get(std::string name) override;
	virtual Config* Get(int index) override;

	virtual Config* GetBase() override;

	virtual void ToString(std::ostringstream& output, std::string prefix = "") override;

	virtual std::string ToString() const override;

public:
	virtual bool Load(std::ifstream& stream) override;
};
