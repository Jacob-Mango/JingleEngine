#pragma once

#include "Editor/Editor.h"

#include "Editor/Editors/GraphEditor.h"

#include "Editor/Editors/Shader/ShaderGraph.h"

class ShaderNode_Output;

class ShaderEditor : public GraphEditor
{
	DEFINE_CLASS(ShaderEditor, GraphEditor);

	friend class ShaderGraph;

private:
	ShaderGraph* m_Graph = nullptr;
	Config* m_Config = nullptr; //! TODO: Reference

	ShaderNode_Output* m_OutputNode = nullptr;

public:
	ShaderEditor();
	~ShaderEditor();

public:
	virtual void New() override;
	virtual void Open(AssetID id) override;
	virtual void SaveAs(AssetID id) override;
	virtual void Save() override;

public:
	virtual void OnRenderMenu() override;

public:
	void Compile();

protected:
	void OnSave();

public:
	virtual Graph* GetGraph() { return m_Graph; }

};
