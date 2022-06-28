#pragma once

#include "Editor/EditorPanelBase.h"

class EntityPropertiesPanel : public EditorPanelBase
{
	DEFINE_CLASS(EntityPropertiesPanel, EditorPanelBase);

public:
	enum class HeaderType
	{
		HEADER,
		CELL
	};

	struct RenderContext
	{
		std::string m_Name;
		bool m_ShowHeader = true;
	};

	struct ArrayContext
	{
		PropertyBase* m_Selected = nullptr;
	};

private:
	static const size_t MaxArrayDepth = 256;
	ArrayContext m_Arrays[MaxArrayDepth];
	size_t m_ArrayIndex;

public:
	EntityPropertiesPanel() {}

	virtual void OnBeginRender(double DeltaTime) override;
	virtual void OnRender(double DeltaTime) override;
	virtual void OnEndRender(double DeltaTime) override;

	void RenderPropertyHeader(const RenderContext& ctx, bool isHeader);

	void RenderPropertyArray(const RenderContext& ctx, class PropertyArray* root);
	void RenderPropertyAsset(const RenderContext& ctx, class PropertyAsset* root);
	void RenderPropertyBase(const RenderContext& ctx, class PropertyBase* root);
	void RenderPropertyItem(const RenderContext& ctx, class PropertyItem* root);
	void RenderPropertyObject(const RenderContext& ctx, class PropertyObject* root);

};
