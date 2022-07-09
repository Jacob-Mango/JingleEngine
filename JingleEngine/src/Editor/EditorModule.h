#pragma once

#include "Core/Core.h"
#include "Core/Module.h"

#include "Entities/Entity.h"

#include "Editor/Editor.h"
#include "Editor/EditorAttribute.h"
#include "Editor/EditorPanel.h"

class EditorModule : public Module
{
	DEFINE_MODULE(EditorModule, Module);

	std::map<JingleScript::Type*, EditorData> m_Editors;

	Entity* m_SelectedEntity = nullptr;

public:
	virtual void OnInitialize() override;

	bool PrepareRender();

	void RenderMenu();
	
	void RenderMain(double DeltaTime, ImGuiID DockspaceId);
	void RenderEditors(double DeltaTime, ImGuiID DockspaceId);

	void OpenAsset(AssetID id);
	Editor* Open(std::string typeName);

	void SetFileName(Editor* editor);

	void SelectEntity(Entity* entity);
	Entity* GetSelectedEntity();
	void ClearSelection();

};
