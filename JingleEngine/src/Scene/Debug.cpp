#include "Debug.h"

void DebugType::Load(Config& config)
{
	Super::Load(config);
}

void Debug::OnSimulate(double DeltaTime)
{
	Super::OnSimulate(DeltaTime);

	Delete();
}
