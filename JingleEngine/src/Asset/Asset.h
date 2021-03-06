#pragma once

#include "Core/Core.h"

#include "AssetID.h"

#define SETUP_ASSET(CLS, base)	\
	DEFINE_CLASS(CLS, base);	\
	friend class AssetModule; 

class Asset : public JingleScript::ManagedObject
{
	DEFINE_CLASS(Asset, JingleScript::ManagedObject);

	friend class AssetModule;

private:
	AssetID m_AssetID;

public:
	Asset();
	virtual ~Asset();

	std::string GetPath() const;
	std::string GetExtension() const;
	AssetID GetAssetID() const;

	virtual bool OnLoad();
	virtual bool OnSave();

	virtual std::string ToString() const override;
};

#include "AssetModule.h"