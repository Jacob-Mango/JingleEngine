#include "Property/Property.h"

BEGIN_CLASS_LINK(Property);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

BEGIN_CLASS_LINK(ArrayProperty);
	LINK_CONSTRUCTOR();
	LINK_CONSTRUCTOR(JingleScript::String);
END_CLASS_LINK();

//ArrayProperty::ArrayProperty(std::string templateType)
//{
//	m_TemplateType = JingleScript::TypeManager::Get(templateType);
//}
