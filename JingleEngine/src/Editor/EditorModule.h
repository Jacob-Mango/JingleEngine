#pragma once

#include "Core/Core.h"
#include "Core/Module.h"

#include "Entities/Entity.h"

class EditorPanelBase;
class EditorAttribute;

class EditorModule : public Module
{
	DEFINE_MODULE(EditorModule, Module);

	struct EditorPanelData
	{
		EditorAttribute* Attribute;

		std::vector<EditorPanelBase*> Instances;
	};

	std::map<JingleScript::Type*, EditorPanelData> m_Panels;

	Entity* m_SelectedEntity;

public:
	virtual void OnInitialize() override;
	virtual void OnEvent(BaseClass* sender, const EventArgs& args) override;

	void RenderMenuBar();

	EditorPanelBase* Open(std::string typeName);

	void SelectEntity(Entity* entity);
	Entity* GetSelectedEntity();
	void ClearSelection();

private:
	int FindLowestNumber(std::vector<EditorPanelBase*> panels, int start, int end);

};
