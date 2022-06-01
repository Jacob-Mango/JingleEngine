#pragma once

#include "Core/Core.h"

#include "GUID.h"

#define SETUP_ASSET(CLS, base)	\
	DEFINE_CLASS(CLS, base);	\
	friend class AssetModule; 

class Asset : public JingleScript::ManagedObject
{
	DEFINE_CLASS(Asset, JingleScript::ManagedObject);

	friend class AssetModule;

private:
	GUID m_GUID;
	bool m_IsLoaded = false;

public:
	Asset();
	virtual ~Asset();

	std::string GetPath() const;
	std::string GetExtension() const;
	GUID GetGUID() const;

	bool IsLoaded() const;

	virtual bool OnLoad();

	virtual std::string ToString() const override;
};

#include "AssetModule.h"