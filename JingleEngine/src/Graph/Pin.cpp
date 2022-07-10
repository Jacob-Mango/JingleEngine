#include "Graph/Pin.h"

BEGIN_CLASS_LINK(Pin)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

Pin::Pin()
{
}

Pin::~Pin()
{
}

BEGIN_CLASS_LINK(InPin)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

InPin::InPin()
{
}

InPin::~InPin()
{
}

BEGIN_CLASS_LINK(OutPin)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

OutPin::OutPin()
{
}

OutPin::~OutPin()
{
}
