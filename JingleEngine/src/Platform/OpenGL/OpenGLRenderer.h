#include "Rendering/RenderAPI.h"

class OpenGLRenderer : public RenderAPI
{
public:
	virtual void Init() override;
	virtual void Destroy() override;

	virtual void RenderFrame() override;
};
