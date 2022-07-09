#include "Editor/EditorModule.h"

#include "Core/Application.h"

#include "Editor/Editor.h"
#include "Editor/EditorAttribute.h"
#include "Editor/EditorPanel.h"

#include <filesystem>

BEGIN_MODULE_LINK(EditorModule);
END_MODULE_LINK();

void EditorModule::OnInitialize()
{
	using namespace JingleScript;

	m_Editors.clear();

	Type* baseType = Editor::StaticType();

	for (auto& type : TypeManager::Iterator())
	{
		if (!type->IsInherited(baseType) || type == baseType)
		{
			continue;
		}

		EditorData data;

		for (auto& attributeBase : type->Attributes)
		{
			if (attributeBase->GetType() != EditorAttribute::StaticType())
			{
				continue;
			}

			data.Attribute = static_cast<EditorAttribute*>(attributeBase);
		}

		m_Editors.insert({ type, data });
	}

	Open("EntityEditor");
}

bool EditorModule::RenderEditors(double DeltaTime, ImGuiID DockspaceId)
{
	bool remainOpen = false;

	for (auto& [type, data] : m_Editors)
	{
		auto& instances = data.Instances;

		for (int i = instances.size() - 1; i >= 0; i--)
		{
			Editor* editor = instances[i];

			if (editor->OnRender(DeltaTime, DockspaceId))
			{
				remainOpen = true;
			}
			else
			{
				//TODO: handle close
			}
		}
	}

	return remainOpen;
}

Editor* EditorModule::Open(std::string typeName)
{
	using namespace JingleScript;

	Type* type = TypeManager::Get(typeName);

	auto it = m_Editors.find(type);
	if (it == m_Editors.end())
	{
		return nullptr;
	}

	auto& data = it->second;

	Editor* editor = type->New<Editor>();
	editor->New();
	data.Instances.push_back(editor);

	return editor;
}

void EditorModule::SetFileName(Editor* editor)
{
	using namespace JingleScript;

	if (!editor->m_AssetID.GetPath().empty())
	{
		std::filesystem::path p = editor->m_AssetID.GetPath();
		editor->m_FileName = p.filename().string();
		return;
	}

	Type* type = TypeManager::Get(editor->GetType()->Name());

	auto it = m_Editors.find(type);
	if (it == m_Editors.end())
	{
		editor->m_FileName = "Not Registered";
		return;
	}

	auto& data = it->second;

	editor->m_FileName = fmt::format("Unsaved {}", data.Instances.size());
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
