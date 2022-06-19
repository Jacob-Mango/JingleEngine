#pragma once

#include "Core/Core.h"

#include "Core/Logging.h"
#include "Core/Module.h"

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

public:
	EditorModule() {}

	virtual void OnInitialize() override;

	virtual void OnEvent(BaseClass* sender, const EventArgs& args) override;

	void RenderMenuBar();

	EditorPanelBase* Open(std::string typeName);

private:
	int FindLowestNumber(std::vector<EditorPanelBase*> panels, int start, int end);

};
