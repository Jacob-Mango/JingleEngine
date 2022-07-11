#include "Editor/Panels/GraphPanel.h"

#include "Editor/Editor.h"

#include <imnodes.h>

BEGIN_CLASS_LINK(GraphPanel);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

GraphPanel::GraphPanel()
{
}

void GraphPanel::OnBeginRender(double DeltaTime)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
}

struct PinInfo
{
	Node* inNode;
	InPin* inPin;

	Node* outNode;
	OutPin* outPin;
};

typedef int ImGuiGraphID;

std::vector<std::pair<Node*, OutPin*>> g_Links;
std::unordered_map<ImGuiGraphID, PinInfo> g_Pins;

size_t UpdatePin(Node* node, Pin* pin)
{
	std::string name = pin->GetName();

	size_t nodeId = reinterpret_cast<size_t>(node);
	size_t pinId = nodeId + std::hash<std::string>()(name);

	auto it = g_Pins.find(pinId);
	PinInfo* info = nullptr;
	if (it == g_Pins.end())
	{
		g_Pins.insert({ (ImGuiGraphID)pinId, PinInfo() });

		it = g_Pins.find(pinId);

		if (it == g_Pins.end())
		{
			return 0;
		}
	}

	info = &it->second;

	InPin* inPin = dynamic_cast<InPin*>(pin);
	OutPin* outPin = dynamic_cast<OutPin*>(pin);

	if (inPin)
	{
		info->inNode = node;
		info->inPin = inPin;
	}
	else if (outPin)
	{
		info->outNode = node;
		info->outPin = outPin;
	}

	return pinId;
}

void GraphPanel::OnRender(double DeltaTime)
{
	g_Links.clear();
	g_Pins.clear();

	ImNodes::BeginNodeEditor();

	Graph* graph = GetEditor()->GetGraph();

	if (ImGui::GetIO().MouseClicked[1])
	{
		ImGui::OpenPopup("GraphPanelContextMenu");
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 8.f));
	if (ImGui::BeginPopup("GraphPanelContextMenu"))
	{
		const ImVec2 position = ImGui::GetMousePosOnOpeningCurrentPopup();

		for (auto& type : JingleScript::TypeManager::Iterator())
		{
			if (!type->IsInherited(graph->GetNodeType()) || type == graph->GetNodeType())
			{
				continue;
			}

			std::string title = fmt::format("Insert {}", type->Name());

			if (ImGui::MenuItem(title.c_str()))
			{
				Node* node = type->New<Node>();
				if (node->Deserialize(nullptr))
				{
					node->OnCreate();
					graph->GetNodes().Insert(node);
				}
				else
				{
					//todo: delete node, mem leak
				}
			}
		}
		ImGui::EndPopup();
	}
    ImGui::PopStyleVar();

	const float nodeWidth = 250.f;

	for (auto& node : graph->GetNodes())
	{
		size_t nodeId = reinterpret_cast<size_t>(node);

		ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(11, 109, 191, 255));
		ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(45, 126, 194, 255));
		ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(81, 148, 204, 255));

		ImNodes::BeginNode((int)nodeId);

		{
			ImNodes::BeginNodeTitleBar();

			const size_t BufferMaxSize = 256;
			char Buffer[BufferMaxSize] = "\0";

			std::string value = node->GetName();

			size_t size = std::min(value.size() + 1, BufferMaxSize);
			memset(Buffer, 0, BufferMaxSize);
			memcpy(Buffer, value.c_str(), size);

			ImGui::PushItemWidth(nodeWidth);
			if (ImGui::InputText(fmt::format("##{}Input", nodeId).c_str(), Buffer, BufferMaxSize, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				value = std::string(Buffer);
				node->SetName(value);
			}

			ImNodes::EndNodeTitleBar();
		}

		size_t inSize = node->m_InPins.size();
		size_t outSize = node->m_OutPins.size();
		size_t size = std::max(inSize, outSize);
		for (size_t index = 0; index < size; index++)
		{
			if (index < inSize)
			{
				auto& pin = node->m_InPins[index];
				std::string name = pin->GetName();
				size_t pinId = UpdatePin(node, pin);
				if (pinId == 0)
				{
					continue;
				}

				ImNodes::BeginInputAttribute((int)pinId);
				ImGui::TextUnformatted(name.c_str());
				ImNodes::EndInputAttribute();
			}

			if (index < outSize && index < inSize)
			{
				ImGui::SameLine();
			}

			if (index < outSize)
			{
				auto& pin = node->m_OutPins[index];
				std::string name = pin->GetName();
				size_t pinId = UpdatePin(node, pin);
				if (pinId == 0)
				{
					continue;
				}

				ImNodes::BeginOutputAttribute((int)pinId);

				const float labelWidth = ImGui::CalcTextSize(name.c_str()).x;
				ImGui::Indent(nodeWidth - labelWidth);
				ImGui::TextUnformatted(name.c_str());
				ImNodes::EndOutputAttribute();
			}
		}

		ImNodes::EndNode();

		for (auto& connection : node->m_Connections)
		{
			auto& nodeA = node;
			auto& nodeB = connection.second.first;

			auto& pinA = connection.first;
			auto& pinB = connection.second.second;

			size_t nodeAId = reinterpret_cast<size_t>(nodeA);
			size_t nodeBId = reinterpret_cast<size_t>(nodeB);

			size_t pinAId = nodeAId + std::hash<std::string>()(pinA->GetName());
			size_t pinBId = nodeBId + std::hash<std::string>()(pinB->GetName());
			
			ImNodes::Link(g_Links.size(), (ImGuiGraphID)pinAId, (ImGuiGraphID)pinBId);

			g_Links.push_back({ nodeA, pinA });
		}

		ImNodes::PopColorStyle();
		ImNodes::PopColorStyle();
		ImNodes::PopColorStyle();
	}

	ImNodes::EndNodeEditor();

	int linkId;
	if (ImNodes::IsLinkDestroyed(&linkId))
	{
		Node* node = g_Links[linkId].first;
		OutPin* pin = g_Links[linkId].second;

		node->m_Connections.erase(node->m_Connections.find(pin));
	}

	int inPinId;
	int outPinId;
	if (ImNodes::IsLinkCreated(&outPinId, &inPinId))
	{
		auto outIt = g_Pins.find(outPinId);
		auto inIt = g_Pins.find(inPinId);

		if (inIt != g_Pins.end() && outIt != g_Pins.end())
		{
			PinInfo& out = outIt->second;
			PinInfo& in = inIt->second;

			out.outNode->m_Connections[out.outPin] = { in.inNode, in.inPin };
		}
	}
}

void GraphPanel::OnEndRender(double DeltaTime)
{
	ImGui::PopStyleVar();
}
