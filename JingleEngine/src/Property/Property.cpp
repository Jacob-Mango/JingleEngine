#include "Property/Property.h"

using namespace JingleScript;

BEGIN_CLASS_LINK(Property);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

BEGIN_CLASS_LINK(ArrayProperty);
	LINK_CONSTRUCTOR(JingleScript::String);
END_CLASS_LINK();

ArrayProperty::ArrayProperty(JingleScript::String templateType)
{
	m_TemplateType = JingleScript::TypeManager::Get(templateType);
}
