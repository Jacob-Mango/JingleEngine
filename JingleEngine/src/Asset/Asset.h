#pragma once

#include <string>

#include "Core/Reference.h"

#include "GUID.h"

#define SETUP_ASSET(CLS, base) friend class AssetManager; private: CLS(const GUID& guid) : base(guid) {}

class Asset : public Countable
{
	typedef Countable base;

	friend class AssetManager;

private:
	GUID m_GUID;
	bool m_IsLoaded = false;

protected:
	Asset(const GUID& guid);

public:
	Asset(const Asset& other) = delete;
	virtual ~Asset() override;

	std::string GetPath();
	std::string GetExtension();
	GUID GetGUID();

	bool IsLoaded();

	virtual bool OnLoad();

	virtual std::string ToString() override;
};

#include "AssetManager.h"