#pragma once

#include "Core/Core.h"
#include "Core/Module.h"

#include "Asset/AssetID.h"

#include "Editor/EditorAttribute.h"
#include "Editor/EditorPanel.h"

#include "Entities/Entity.h"

class EditorModule;

class Editor : public JingleScript::Object
{
	DEFINE_CLASS(Editor, JingleScript::Object);

	friend EditorModule;

private:
	bool m_ShouldClose = false;

	AssetID m_AssetID;
	std::string m_FileName;

	EditorModule* m_Module;

	std::map<JingleScript::Type*, EditorPanelData> m_Panels;

public:
	Editor();
	~Editor();

	bool OnRender(double DeltaTime, ImGuiID DockspaceId);
	void Close();

	EditorModule* GetEditorModule() const { return m_Module; }
	const AssetID& GetAssetID() const { return m_AssetID; }
	std::string GetID() const { return m_AssetID.GetPath() + "-" + m_FileName; }
	
public:
	virtual void New() {}
	virtual void Open(AssetID id) {}
	virtual void SaveAs(AssetID id) {}
	virtual void Save() {}

public:
	virtual void OnRenderMenu();

public:
	// Generic functions across editors for panels

	virtual Entity* GetEntity() { return nullptr; }

public:
	EditorPanel* OpenPanel(const std::string& typeName);

protected:
	void SetAssetID(AssetID id) { m_AssetID = id; }

};

struct EditorData
{
	EditorAttribute* Attribute;

	std::vector<Editor*> Instances;
};
