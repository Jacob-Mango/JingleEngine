#include "Property/Property.h"

using namespace JingleScript;

BEGIN_CLASS_LINK(Property);
	LINK_CONSTRUCTOR();
	LINK_CONSTRUCTOR(bool);
	LINK_CONSTRUCTOR(std::string);
END_CLASS_LINK();

BEGIN_CLASS_LINK(ClassProperty);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();
