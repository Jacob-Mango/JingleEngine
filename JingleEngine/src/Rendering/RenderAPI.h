#pragma once

#include "Core/Core.h"

class RenderAPI
{
public:
	virtual void Init() = 0;
	virtual void Destroy() = 0;

	virtual void RenderFrame() = 0;
};
