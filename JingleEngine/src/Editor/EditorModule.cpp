#include "Editor/EditorModule.h"

#include "Editor/EditorAttribute.h"
#include "Editor/EditorPanelBase.h"

BEGIN_MODULE_LINK(EditorModule);
END_MODULE_LINK();

void EditorModule::OnInitialize()
{
	using namespace JingleScript;

	m_Panels.clear();

	Type* baseType = EditorPanelBase::StaticType();

	for (auto& type : TypeManager::Iterator())
	{
		if (!type->IsInherited(baseType) || type == baseType)
		{
			continue;
		}

		EditorPanelData data;

		for (auto& attributeBase : type->Attributes)
		{
			if (attributeBase->GetType() != EditorAttribute::StaticType())
			{
				continue;
			}

			data.Attribute = static_cast<EditorAttribute*>(attributeBase);
		}

		m_Panels.insert({ type, data});
	}

	Open("EditorViewportPanel");
	Open("EntityPropertiesPanel");
	Open("SceneHierarchyPanel");
	Open("ContentBrowserPanel");
}

STATIC_FUNCTION(EditorPanelBase, OnBeginRender, void, double);
STATIC_FUNCTION(EditorPanelBase, OnRender, void, double);
STATIC_FUNCTION(EditorPanelBase, OnEndRender, void, double);

void EditorModule::OnEvent(BaseClass* sender, const EventArgs& args)
{
	switch (args.GetType())
	{
		case EventTypeImGuiRender:
			double deltaTime = args.As<ImGuiRenderEventArgs>().DeltaTime;

			for (auto& [type, data] : m_Panels)
			{
				auto& instances = data.Instances;

				for (int i = instances.size() - 1; i >= 0; i--)
				{
					EditorPanelBase* panel = instances[i];

					std::string title = data.Attribute->GetTitle();

					if (panel->m_OpenedIndex > 0)
					{
						title = title + " #" + std::to_string(panel->m_OpenedIndex);
					}

					bool isOpen = true;
					bool canClose = true;

					Script_OnBeginRender[panel](deltaTime);

					if (ImGui::Begin(title.c_str(), &isOpen))
					{
						Script_OnRender[panel](deltaTime);
					}

					Script_OnEndRender[panel](deltaTime);

					panel->m_ShouldClose |= !isOpen;
					if (panel->m_ShouldClose)
					{
						//! TODO: Handle logic that prevents closing such as "Unsaved data" prompt. Will set 'canClose' to 'false'
					}

					ImGui::End();

					if (panel->m_ShouldClose && canClose)
					{
						//JingleScript::DeleteObject(instances[i]);

						instances.erase(instances.begin() + i);
					}
				}
			}
			
			break;
	}
}

void EditorModule::RenderMenuBar()
{
	if (ImGui::BeginMenu("Panels"))
	{
		for (auto& [type, data] : m_Panels)
		{
			std::string title = data.Attribute->GetTitle();

			if (ImGui::MenuItem(title.c_str(), NULL))
			{
				Open(type->Name());
			}
		}

		ImGui::EndMenu();
	}
}

int EditorModule::FindLowestNumber(std::vector<EditorPanelBase*> panels, int start, int end)
{
	if (start > end)
		return end + 1;

	if (start != panels[start]->m_OpenedIndex)
		return start;

	int mid = (start + end) / 2;

	// Left half has all elements from 0 to mid
	if (panels[mid]->m_OpenedIndex == mid)
		return FindLowestNumber(panels, mid + 1, end);

	return FindLowestNumber(panels, start, mid);
}

EditorPanelBase* EditorModule::Open(std::string typeName)
{
	using namespace JingleScript;

	Type* type = TypeManager::Get(typeName);

	auto it = m_Panels.find(type);
	if (it == m_Panels.end())
	{
		return nullptr;
	}

	auto& data = it->second;

	int lowest = FindLowestNumber(data.Instances, 0, data.Instances.size() - 1);

	EditorPanelBase* panel = type->New<EditorPanelBase>();
	panel->m_OpenedIndex = lowest;
	data.Instances.push_back(panel);
	return panel;
}

void EditorModule::SelectEntity(Entity* entity)
{
	m_SelectedEntity = entity;
}

Entity* EditorModule::GetSelectedEntity()
{
	return m_SelectedEntity;
}

void EditorModule::ClearSelection()
{
	m_SelectedEntity = nullptr;
}
