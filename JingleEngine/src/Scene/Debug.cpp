#include "Debug.h"

void DebugType::Load(Config& config)
{
	super::Load(config);
}

void Debug::OnSimulate(double DeltaTime)
{
	super::OnSimulate(DeltaTime);

	Delete();
}
