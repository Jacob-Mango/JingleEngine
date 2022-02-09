#include "Light.h"

#include "Shader.h"

#include <sstream>

void LightType::Load(Config& config)
{
	super::Load(config);

	switch (config["type"].Int)
	{
		case 1:
			Type = Type::DIRECTIONAL;
			break;
		case 2:
			Type = Type::POINT;
			break;
	}

	Color = config["color"].Vec3();

	Constant = config["constant"].Float;
	Linear = config["linear"].Float;
	Quadratic = config["quadratic"].Float;
}

void Light::OnCreate()
{
	super::OnCreate();

	GetScene()->AddLight(this);
}

void Light::OnDestroy()
{
	super::OnDestroy();

	GetScene()->RemoveLight(this);
}

void Light::Process(Ref<Shader> shader, int& pointIdx, int& directionalIdx)
{
	std::string prefix = "";

	switch (GetType()->Type)
	{
	case LightType::Type::POINT:
		prefix = "u_PointLights[" + std::to_string(pointIdx) + "].";
		pointIdx++;

		shader->Set(prefix + "constant", GetType()->Constant);
		shader->Set(prefix + "linear", GetType()->Linear);
		shader->Set(prefix + "quadratic", GetType()->Quadratic);

		break;
	case LightType::Type::DIRECTIONAL:
		prefix = "u_DirectionalLights[" + std::to_string(directionalIdx) + "].";
		directionalIdx++;

		GetUpDirection();
		shader->Set(prefix + "direction", glm::vec3(GetWorldTransform()[2]));

		break;
	}

	shader->Set(prefix + "position", glm::vec3(GetWorldTransform()[3]));
	shader->Set(prefix + "color", GetType()->Color);
}

std::string Light::ToString()
{
	std::stringstream ss;

	ss << base::ToString();

	return ss.str();
}

std::string LightType::ToString()
{
	std::stringstream ss;

	ss << base::ToString();

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
