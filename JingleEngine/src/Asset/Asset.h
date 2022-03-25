#pragma once

#include "Core/Core.h"

#include "GUID.h"

#define SETUP_ASSET(CLS, base) friend class AssetModule; private: CLS(const GUID& guid) : base(guid) {}

class Asset : public Countable
{
	typedef Countable Super;

	friend class AssetModule;

private:
	GUID m_GUID;
	bool m_IsLoaded = false;

protected:
	Asset(const GUID& guid);

public:
	Asset(const Asset& other) = delete;
	virtual ~Asset() override;

	std::string GetPath() const;
	std::string GetExtension() const;
	GUID GetGUID() const;

	bool IsLoaded() const;

	virtual bool OnLoad();

	virtual std::string ToString() const override;
};

#include "AssetModule.h"