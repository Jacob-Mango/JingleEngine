#pragma once

#include "Core/Core.h"

#include <string>

typedef unsigned long long int AssetIDv;

class AssetID
{
	friend class Asset;

private:
	AssetIDv m_Value;
	std::string m_Path;

public:
	AssetID();
	AssetID(AssetIDv id);
	AssetID(std::string path);
	AssetID(const AssetID& other);

	std::string GetPath() const;

	AssetIDv GetValue() const;

	operator AssetIDv() const;
};
