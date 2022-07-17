#ifndef IMGUI_DEFINE_MATH_OPERATORS
	#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "Editor/Panels/GraphPanel.h"

#include "Editor/Editors/GraphEditor.h"

#include <imnodes.h>
#include <imnodes_internal.h>

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

std::vector<Edge> g_Links;

std::unordered_map<ImGuiGraphID, Node*> g_Nodes;
std::unordered_map<ImGuiGraphID, PinInfo> g_Pins;

std::vector<int> g_SelectedLinks;
std::vector<int> g_SelectedNodes;

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
	GraphEditor* editor = dynamic_cast<GraphEditor*>(GetEditor());
	if (!editor)
	{
		return;
	}

	Graph* graph = editor->GetGraph();

	ImNodes::BeginNodeEditor();

	const float nodeWidth = 250.f;

	ImNodesContext* nodesCtx = ImNodes::GetCurrentContext();
	ImNodesEditorContext* nodeEditorCtx = nodesCtx->EditorCtx;
	ImObjectPool<ImNodeData>& nodes = nodeEditorCtx->Nodes;

	if (ImGui::GetIO().MouseClicked[1])
	{
		ImGui::OpenPopup("GraphPanelContextMenu");
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 8.f));
	if (ImGui::BeginPopup("GraphPanelContextMenu"))
	{
		ImVec2 position = ImGui::GetMousePosOnOpeningCurrentPopup();

		if (ImGui::BeginMenu("Insert"))
		{
			for (auto& type : JingleScript::TypeManager::Iterator())
			{
				if (!type->IsInherited(graph->GetNodeType()) || type == graph->GetNodeType())
				{
					continue;
				}

				std::string title = fmt::format("{}", type->Name());

				if (ImGui::MenuItem(title.c_str()))
				{
					Node* node = graph->CreateNode<Node>(type);

					ImVec2 pos = position - (nodesCtx->CanvasOriginScreenSpace - nodeEditorCtx->Panning);

					node->m_EditorPositionX = pos.x;
					node->m_EditorPositionY = pos.y;
				}
			}

			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Delete"))
		{
			for (auto& [out, in] : editor->SelectedEdges)
			{
				out.first->DeleteConnection(out.second, in);
			}

			for (auto& node : editor->SelectedNodes)
			{
				graph->RemoveNode(node);
			}
		}

		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();

	g_Links.clear();
	g_Nodes.clear();
	g_Pins.clear();

	ImGuiGraphID linkId = 0;
	for (auto& node : graph->GetNodes())
	{
		ImGuiGraphID nodeId = reinterpret_cast<size_t>(node);
		g_Nodes[nodeId] = node;

		ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(11, 109, 191, 255));
		ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(45, 126, 194, 255));
		ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(81, 148, 204, 255));

		int nodeIdx = nodes.IdMap.GetInt(static_cast<ImGuiID>((int)nodeId), -1);

		ImVec2 position = { (float)node->m_EditorPositionX, (float)node->m_EditorPositionY };
		ImNodes::SetNodeGridSpacePos((int)nodeId, position);

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

		size_t inSize = node->m_temp_InPins.size();
		size_t outSize = node->m_temp_OutPins.size();
		size_t size = std::max(inSize, outSize);
		for (size_t index = 0; index < size; index++)
		{
			if (index < inSize)
			{
				auto& pin = node->m_temp_InPins[index];
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
				auto& pin = node->m_temp_OutPins[index];
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

		for (auto& outConnection : node->m_OutConnections)
		{
			auto& nodeA = node;
			auto& pinA = outConnection.first;

			size_t nodeAId = reinterpret_cast<size_t>(nodeA);
			size_t pinAId = nodeAId + std::hash<std::string>()(pinA->GetName());

			for (auto& inConnection : outConnection.second)
			{
				auto& nodeB = inConnection.first;
				auto& pinB = inConnection.second;

				size_t nodeBId = reinterpret_cast<size_t>(nodeB);
				size_t pinBId = nodeBId + std::hash<std::string>()(pinB->GetName());

				ImNodes::Link(linkId, (ImGuiGraphID)pinAId, (ImGuiGraphID)pinBId);

				g_Links.resize(static_cast<size_t>(linkId + 1));
				g_Links[linkId] = { { nodeA, pinA }, { nodeB, pinB } };

				linkId++;
			}
		}

		ImNodes::PopColorStyle();
		ImNodes::PopColorStyle();
		ImNodes::PopColorStyle();
	}

	ImNodes::EndNodeEditor();

	for (auto& node : graph->GetNodes())
	{
		ImGuiGraphID nodeId = reinterpret_cast<size_t>(node);

		ImVec2 position = ImNodes::GetNodeGridSpacePos((int)nodeId);
		node->m_EditorPositionX = position.x;
		node->m_EditorPositionY = position.y;
	}

	if (ImNodes::IsLinkDestroyed(&linkId))
	{
		Edge& edge = g_Links[linkId];

		edge.first.first->DeleteConnection(edge.first.second, edge.second);
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

			out.outNode->CreateConnection(out.outPin, { in.inNode, in.inPin });
		}
	}

	{
		size_t count = static_cast<size_t>(ImNodes::NumSelectedLinks());
		g_SelectedLinks.resize(count);
		editor->SelectedEdges.resize(count);
		if (count != 0)
		{
			ImNodes::GetSelectedLinks(g_SelectedLinks.data());
		}
		
		size_t index = 0;
		for (auto id : g_SelectedLinks)
		{
			if (id >= g_Links.size()) continue;
			editor->SelectedEdges[index++] = g_Links[id];
		}
	}

	{
		size_t count = static_cast<size_t>(ImNodes::NumSelectedNodes());
		g_SelectedNodes.resize(count);
		editor->SelectedNodes.resize(count);
		if (count != 0)
		{
			ImNodes::GetSelectedNodes(g_SelectedNodes.data());
		}

		size_t index = 0;
		for (auto id : g_SelectedNodes)
		{
			editor->SelectedNodes[index++] = g_Nodes[id];
		}
	}
}

void GraphPanel::OnEndRender(double DeltaTime)
{
	ImGui::PopStyleVar();
}
