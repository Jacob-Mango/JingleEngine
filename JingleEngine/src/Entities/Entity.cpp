#include "Entity.h"

#include <glm/gtx/euler_angles.hpp>

#include "Asset/AssetModule.h"

#include "Core/Application.h"

#include "Entities/Component.h"
#include "Entities/Components/MeshComponent.h"

#include "Rendering/Mesh.h"
#include "Rendering/Material.h"

BEGIN_CLASS_LINK(EntityArray)
	LINK_CONSTRUCTOR();
	Array<Entity*>::ScriptRegister(type);
END_CLASS_LINK()

BEGIN_CLASS_LINK(Entity)
	LINK_NAMED_VARIABLE(dquat4, m_Transform);
	LINK_NAMED_VARIABLE(Components, m_Components);
	LINK_NAMED_VARIABLE(Children, m_Children);
	LINK_CONSTRUCTOR();
	LINK_METHOD(OnCreate);
	LINK_METHOD(OnDestroy);
	LINK_METHOD(OnTick);
	LINK_METHOD(AddChild);
	LINK_METHOD(RemoveChild);
	LINK_METHOD(GetParent);
	LINK_METHOD(OnSerializeTransform);
	LINK_METHOD(OnDeserializeTransform);
	LINK_METHOD(Editor_OnRenderTransform);
	LINK_METHOD(OnSerializeComponents);
	LINK_METHOD(OnDeserializeComponents);
	LINK_METHOD(Editor_OnRenderComponents);
	LINK_METHOD(OnSerializeChildren);
	LINK_METHOD(OnDeserializeChildren);
	LINK_METHOD(Editor_OnRenderChildren);
	LINK_METHOD(Editor_OnPropertyChanged);
END_CLASS_LINK()

Entity::Entity()
{
	m_BoundingBox[0] = glm::vec3(-1);
	m_BoundingBox[1] = glm::vec3(1);
}

Entity::~Entity()
{
	if (!m_IsDeleting)
	{
		JS_ERROR("Entity being deleted without being called by Entity::Delete!");
	}
}

void Entity::AddComponent(Component* component)
{
	using namespace JingleScript;

	component->m_Entity = this;
	m_Components->Insert(component);
}

ComponentArray& Entity::GetComponents()
{
	return *m_Components;
}

void Entity::Delete()
{
	if (m_IsDeleting) return;
	m_IsDeleting = true;

	OnDestroy();

	for (int i = m_Children->Count() - 1; i >= 0; i--)
	{
		//RemoveChild(m_Children[i]);
	}
}

bool Entity::IsDeleting()
{
	return m_IsDeleting;
}

void Entity::Editor_OnPropertyChanged(std::string name)
{
	ObjectProperty::Editor_OnPropertyChanged(name);
}

void Entity::OnSerializeTransform(Config* cfg)
{
	cfg->CreateValue("Position", js_dvec3(GetPosition()).ToString());
	cfg->CreateValue("Orientation", js_vec3(GetOrientation()).ToString());
}

void Entity::OnDeserializeTransform(Config* cfg)
{
	SetPosition(js_dvec3::FromString(cfg->GetValue("Position")));
	SetOrientation(js_vec3::FromString(cfg->GetValue("Orientation")));
}

void Entity::Editor_OnRenderTransform()
{
	EditorUI::Render_CellHeader("Position");
	glm::dvec3 position = GetPosition();
	if (EditorUI::Render_Vector3(position))
	{
		SetPosition(position);
	}

	EditorUI::Render_CellHeader("Orientation");
	glm::vec3 orientation = GetOrientation();
	if (EditorUI::Render_Vector3(orientation))
	{
		SetOrientation(orientation);
	}
}

void Entity::OnSerializeComponents(Config* cfgRoot)
{
	auto cfg = cfgRoot->CreateArray("Components");

	for (auto& component : *m_Components)
	{
		cfg->Set(component->Serialize());
	}
}

void Entity::OnDeserializeComponents(Config* cfgRoot)
{
	auto cfg = cfgRoot->Get("Components");
	if (!cfg)
	{
		return;
	}

	for (auto& cfgComponent : *cfg)
	{
		Component* component = JingleScript::NewObject<Component>(cfgComponent.GetLinkedType());
		component->m_Entity = this;

		component->Deserialize(&cfgComponent);

		if (component->GetName().empty())
		{
			component->SetName(component->GetType()->Name());
		}

		component->OnCreate();

		AddComponent(component);
	}
}

void Entity::Editor_OnRenderComponents()
{
	static Component* SelectedComponent = nullptr;

	EditorUI::Render_CellHeader("Components", true);

	ImGui::TableNextColumn();

	float width = ImGui::GetColumnWidth() * 0.5f;
	float height = ImGui::GetFrameHeight();

	bool addPressed = ImGui::Button("Add", { width, height });
	ImGui::SameLine();
	bool removePressed = ImGui::Button("Remove", { width, height });
	size_t count = GetComponents().size();
	size_t removeIndex = count;

	size_t index = 0;
	for (auto& component : GetComponents())
	{
		ImGui::PushID(std::to_string(index).c_str());

		std::string componentName = component->GetName();

		EditorUI::Render_CellHeader(fmt::format("[{}]", index).c_str(), true, true);

		ImGui::TableNextColumn();
		
		ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_None;

		//! We aren't editting the root config so we can't change it's name
		if (component->HasBase())
		{
			inputFlags = ImGuiInputTextFlags_ReadOnly;
		}

		//ImGuiInputTextFlags_EnterReturnsTrue
		bool selected = SelectedComponent == component;
		if (EditorUI::Render_SelectableInput(selected, ImGuiSelectableFlags_SpanAllColumns, inputFlags, componentName))
		{
			SelectedComponent = component;

			component->SetName(componentName);
		}

		component->Editor_Render();

		ImGui::PopID();

		if (SelectedComponent == component && !component->HasBase())
		{
			removeIndex = index;
		}

		index++;
	}

	if (addPressed)
	{
		JingleScript::Type* componentType = MeshComponent::StaticType();
		Component* component = componentType->New<Component>();
		component->m_Entity = this;
		component->Deserialize(nullptr);
		component->SetName(component->GetType()->Name());
		component->OnCreate();

		AddComponent(component);
	}

	if (removePressed)
	{
		// removeIndex
	}
}

void Entity::OnSerializeChildren(Config* cfgRoot)
{
	auto cfg = cfgRoot->CreateArray("Children");

	for (auto& child : *m_Children)
	{
		cfg->Set(child->Serialize());
	}
}

void Entity::OnDeserializeChildren(Config* cfgRoot)
{
	auto cfg = cfgRoot->Get("Children");
	if (!cfg)
	{
		return;
	}

	for (auto& cfgChild : *cfg)
	{
		Entity* entity = JingleScript::NewObject<Entity>(cfgChild.GetLinkedType());

		entity->Deserialize(&cfgChild);

		AddChild(entity);
	}
}

void Entity::Editor_OnRenderChildren()
{
}

void Entity::AddChild(Entity* child)
{
	if (child->m_Parent != nullptr)
	{
		if (child->m_Parent == this)
			return;

		child->m_Parent->RemoveChild(child);
	}

	child->m_Parent = this;
	m_Children->Insert(child);
}

void Entity::RemoveChild(Entity* child)
{
	for (size_t i = 0; i < m_Children->Count(); i++)
	{
		if (m_Children->Get(i) != child)
			continue;

		child->m_Parent = nullptr;
		m_Children->Remove(i);
	}
}

Entity* Entity::GetParent()
{
	return m_Parent;
}

void Entity::SetTransform(glm::dmat4 transform)
{
	m_Transform = transform;
}

glm::dmat4 Entity::GetTransform()
{
	return m_Transform;
}

glm::dmat4 Entity::GetWorldTransform()
{
	glm::dmat4 transform(1.0);

	Entity* parent = this;
	while (parent != nullptr)
	{
		transform = parent->GetTransform() * transform;
		parent = parent->GetParent();
	}

	return transform;
}

void Entity::SetPosition(glm::dvec3 position)
{
	m_Transform[3][0] = position[0];
	m_Transform[3][1] = position[1];
	m_Transform[3][2] = position[2];
	SetTransform(m_Transform);
}

glm::dvec3 Entity::GetPosition() const
{
	return glm::dvec3(m_Transform[3]);
}

glm::dvec3 Entity::GetRightDirection() const
{
	return glm::dvec3(m_Transform[0]);
}

glm::dvec3 Entity::GetUpDirection() const
{
	return glm::dvec3(m_Transform[1]);
}

glm::dvec3 Entity::GetForwardDirection() const
{
	return glm::dvec3(m_Transform[2]);
}

void Entity::SetOrientation(glm::vec3 orientation)
{
	glm::dmat4 trans = glm::eulerAngleYXZ(glm::radians(orientation.x), glm::radians(orientation.y), glm::radians(orientation.z));
	trans[3] = m_Transform[3];
	SetTransform(trans);
}

glm::vec3 Entity::GetOrientation() const
{
	glm::vec3 orientation = glm::vec3();
	glm::extractEulerAngleYXZ(glm::mat4(m_Transform), orientation.x, orientation.y, orientation.z);
	return glm::degrees(orientation);
}

std::string Entity::ToString() const
{
	std::stringstream ss;

	ss << Super::ToString();

	ss << ", ";
	//ss << "Position=" << GetPosition();

	ss << ", ";
	//ss << "Orientation=" << GetOrientation();

	return ss.str();
}

void Entity::SetVisible(bool visible)
{
	m_IsVisible = visible;
}

bool Entity::IsVisible()
{
	return m_IsVisible;
}

void Entity::OnCreate()
{
}

void Entity::OnDestroy()
{
}

void Entity::OnTick(double DeltaTime)
{
}

Entity* Entity::Create(AssetID asset)
{
	ConfigAsset* cfgAsset = AssetModule::Get<ConfigAsset>(asset);
	ConfigSection* cfg = cfgAsset->Get();

	JS_INFO("Entity Create");
	cfgAsset->Output();

	Entity* entity = JingleScript::NewObject<Entity>(cfg->GetLinkedType());
	if (!entity)
	{
		return nullptr;
	}

	cfgAsset->Deserialize(entity);
	return entity;
}
