#include "Debug.h"

BEGIN_CLASS_LINK(DebugType)
END_CLASS_LINK()

BEGIN_CLASS_LINK(Debug)
END_CLASS_LINK()

void DebugType::Load(Config& config)
{
	Super::Load(config);
}

void Debug::OnTick(double DeltaTime)
{
	Super::OnTick(DeltaTime);

	Delete();
}
