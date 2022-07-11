#pragma once

#include "Editor/Editor.h"

#include "Editor/Editors/Shader/ShaderGraph.h"

class ShaderEditor : public Editor
{
	DEFINE_CLASS(ShaderEditor, Editor);

private:
	ShaderGraph* m_Graph = nullptr;
	Config* m_Config = nullptr; //! TODO: Reference

public:
	ShaderEditor();
	~ShaderEditor();

public:
	virtual void New() override;
	virtual void Open(AssetID id) override;
	virtual void SaveAs(AssetID id) override;
	virtual void Save() override;

protected:
	void OnSave();

public:
	virtual Graph* GetGraph() { return m_Graph; }

};
