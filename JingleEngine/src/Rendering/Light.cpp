#include "Light.h"

#include "Shader.h"

#include <sstream>

BEGIN_CLASS_LINK(Light)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

void Light::OnCreate()
{
	Super::OnCreate();

	GetScene()->AddLight(this);
}

void Light::OnDestroy()
{
	Super::OnDestroy();

	GetScene()->RemoveLight(this);
}

void Light::Process(Ref<Shader> shader, int& pointIdx, int& directionalIdx)
{
	std::string prefix = "";

	switch (Type)
	{
	case Type::POINT:
		prefix = "u_PointLights[" + std::to_string(pointIdx) + "].";
		pointIdx++;

		shader->Set(prefix + "constant", Constant);
		shader->Set(prefix + "linear", Linear);
		shader->Set(prefix + "quadratic", Quadratic);

		break;
	case Type::DIRECTIONAL:
		prefix = "u_DirectionalLights[" + std::to_string(directionalIdx) + "].";
		directionalIdx++;

		GetUpDirection();
		shader->Set(prefix + "direction", glm::vec3(GetWorldTransform()[2]));

		break;
	}

	shader->Set(prefix + "position", glm::vec3(GetWorldTransform()[3]));
	shader->Set(prefix + "color", Color);
}

std::string Light::ToString() const
{
	std::stringstream ss;

	ss << Super::ToString();

	ss << ", ";
	ss << "Type=" << (Type == Type::DIRECTIONAL ? "DIRECTIONAL" : "POINT");

	ss << ", ";
	ss << "Color=" << Color;

	ss << ", ";
	ss << "Constant=" << Constant;

	ss << ", ";
	ss << "Linear=" << Linear;

	ss << ", ";
	ss << "Quadratic=" << Quadratic;

	return ss.str();
}
