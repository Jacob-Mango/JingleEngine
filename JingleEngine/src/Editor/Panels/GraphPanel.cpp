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

std::vector<std::pair<Node*, OutPin*>> g_Links;
std::unordered_map<size_t, PinInfo> g_Pins;

size_t UpdatePin(Node* node, Pin* pin)
{
	std::string name = pin->GetName();

	size_t nodeId = reinterpret_cast<size_t>(node);
	size_t pinId = nodeId + std::hash<std::string>()(name);

	auto it = g_Pins.find(pinId);
	PinInfo* info = nullptr;
	if (it == g_Pins.end())
	{
		info = &(g_Pins.insert({ pinId, PinInfo() }).first->second);
	}
	else
	{
		info = &it->second;
	}

	if (InPin* inPin = dynamic_cast<InPin*>(pin))
	{
		info->inNode = node;
		info->inPin = inPin;
	}

	if (OutPin* outPin = dynamic_cast<OutPin*>(pin))
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

	for (auto& node : graph->GetNodes())
	{
		size_t nodeId = reinterpret_cast<size_t>(node);

		ImNodes::BeginNode((int)nodeId);

		ImNodes::BeginNodeTitleBar();
		ImGui::TextUnformatted(node->GetName().c_str());
		ImNodes::EndNodeTitleBar();

		for (auto& pin : node->m_InPins)
		{
			std::string name = pin->GetName();
			size_t pinId = UpdatePin(node, pin);

			ImNodes::BeginInputAttribute((int)pinId);
			ImGui::TextUnformatted(name.c_str());
			ImNodes::EndInputAttribute();
		}

		for (auto& pin : node->m_OutPins)
		{
			std::string name = pin->GetName();
			size_t pinId = UpdatePin(node, pin);

			ImNodes::BeginOutputAttribute((int)pinId);
			ImGui::TextUnformatted(name.c_str());
			ImNodes::EndOutputAttribute();
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
			
			ImNodes::Link(g_Links.size(), (int)pinAId, (int)pinBId);

			g_Links.push_back({ nodeA, pinA });
		}
	}

	ImNodes::EndNodeEditor();

	int linkId;
	if (ImNodes::IsLinkDestroyed(&linkId))
	{
		Node* node = g_Links[linkId].first;
		OutPin* pin = g_Links[linkId].second;

		node->m_Connections[pin] = {};
	}

	int inPinId;
	int outPinId;
	if (ImNodes::IsLinkCreated(&inPinId, &outPinId))
	{
		PinInfo& in = g_Pins[inPinId];
		PinInfo& out = g_Pins[outPinId];

		out.outNode->m_Connections[out.outPin] = { in.inNode, in.inPin };
	}
}

void GraphPanel::OnEndRender(double DeltaTime)
{
	ImGui::PopStyleVar();
}
