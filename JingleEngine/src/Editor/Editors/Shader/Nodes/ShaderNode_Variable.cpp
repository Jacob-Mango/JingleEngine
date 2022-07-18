#include "ShaderNode_Variable.h"

BEGIN_CLASS_LINK(ShaderNode_Variable)
	ADD_ATTRIBUTE(Property);
	LINK_VARIABLE(Name);

	ADD_ATTRIBUTE(OutPin);
	ADD_ATTRIBUTE(Property, true);
	LINK_VARIABLE(Output);

	LINK_CONSTRUCTOR();

	LINK_METHOD(OnSerializeVariableType);
	LINK_METHOD(OnDeserializeVariableType);
	LINK_METHOD(Editor_OnRenderVariableType);
END_CLASS_LINK()

void ShaderNode_Variable::Compile(std::unordered_map<std::string, std::pair<ShaderNode*, std::string>>& inputs, std::stringstream& output)
{

}

void ShaderNode_Variable::OnSerializeVariableType(Config* cfg)
{
	Output = JingleScript::TypeManager::Get(cfg->GetValue("Type"));
}

void ShaderNode_Variable::OnDeserializeVariableType(Config* cfg)
{
	cfg->SetValue("Type", Output ? Output->Name() : "");
}

void ShaderNode_Variable::Editor_OnRenderVariableType()
{
	//! TODO: render a combo box
}
