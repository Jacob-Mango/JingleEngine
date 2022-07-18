#pragma once

#include "Editor/Editor.h"

#include "Editor/Editors/GraphEditor.h"

#include "Editor/Editors/Shader/ShaderGraph.h"

class ShaderEditor : public GraphEditor
{
	DEFINE_CLASS(ShaderEditor, GraphEditor);

	friend class ShaderGraph;

private:
	Shader* m_Shader = nullptr;

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

protected:
	void OnSave();

public:
	virtual Graph* GetGraph() { return m_Shader ? m_Shader->GetGraph() : nullptr; }

};
