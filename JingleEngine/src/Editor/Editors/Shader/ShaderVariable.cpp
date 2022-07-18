#include "Editor/Editors/Shader/ShaderVariable.h"

BEGIN_CLASS_LINK(ShaderVariable)

	LINK_CONSTRUCTOR();
END_CLASS_LINK()

BEGIN_CLASS_LINK(ShaderVariableArray)
	LINK_CONSTRUCTOR();
	Array<ShaderVariable*>::ScriptRegister(type);
END_CLASS_LINK()

ShaderVariable::ShaderVariable()
{

}

ShaderVariable::~ShaderVariable()
{

}
