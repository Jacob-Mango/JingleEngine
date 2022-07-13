#include "ShaderNode_Color.h"

BEGIN_CLASS_LINK(ShaderNode_Color)
	ADD_ATTRIBUTE(OutPin);
	LINK_VARIABLE(Output);
	
	LINK_CONSTRUCTOR();
END_CLASS_LINK()
