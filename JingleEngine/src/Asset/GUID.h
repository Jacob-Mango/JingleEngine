#pragma once

#include <string>

typedef unsigned long long int GUIDv;

class GUID
{
private:
	GUIDv m_Value;
	std::string m_Path;

public:
	GUID(GUIDv id);
	GUID(std::string path);

	std::string GetPath() const;

	GUIDv GetValue() const;

	operator GUIDv() const;
};
