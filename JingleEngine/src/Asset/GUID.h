#pragma once

#include "Core/Core.h"

#include <string>

typedef unsigned long long int GUIDv;

class GUID
{
	friend class Asset;

private:
	GUIDv m_Value;
	std::string m_Path;

private:
	GUID();

public:
	GUID(GUIDv id);
	GUID(std::string path);
	GUID(const GUID& other);

	std::string GetPath() const;

	GUIDv GetValue() const;

	operator GUIDv() const;
};
