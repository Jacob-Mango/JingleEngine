#pragma once

#include "Editor/Editor.h"

#include "Entities/Entity.h"

class EntityEditor : public Editor
{
	DEFINE_CLASS(EntityEditor, Editor);

private:
	Entity* m_Entity = nullptr;
	Config* m_Config = nullptr; //! TODO: Reference

public:
	EntityEditor();
	~EntityEditor();

public:
	virtual void New() override;
	virtual void Open(AssetID id) override;
	virtual void SaveAs(AssetID id) override;
	virtual void Save() override;

public:
	virtual Entity* GetEntity() { return m_Entity; }

};
