#pragma once

#include "Editor/EditorPanelBase.h"

class EntityPropertiesPanel : public EditorPanelBase
{
	DEFINE_CLASS(EntityPropertiesPanel, EditorPanelBase);

public:
	EntityPropertiesPanel() {}

	virtual void OnBeginRender(double DeltaTime) override;
	virtual void OnRender(double DeltaTime) override;
	virtual void OnEndRender(double DeltaTime) override;

	void RenderPropertyArray(const std::string& name, class PropertyArray* root);
	void RenderPropertyAsset(const std::string& name, class PropertyAsset* root);
	void RenderPropertyBase(const std::string& name, class PropertyBase* root);
	void RenderPropertyItem(const std::string& name, class PropertyItem* root);
	void RenderPropertyObject(const std::string& name, class PropertyObject* root);

};
