#include "Light.h"

#include "Shader.h"

#include <sstream>

BEGIN_CLASS_LINK(LightType)
END_CLASS_LINK()

BEGIN_CLASS_LINK(Light)
END_CLASS_LINK()

void LightType::Load(Config& config)
{
	Super::Load(config);

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

	auto& type = GetEntityType<LightType>();

	switch (type.Type)
	{
	case LightType::Type::POINT:
		prefix = "u_PointLights[" + std::to_string(pointIdx) + "].";
		pointIdx++;

		shader->Set(prefix + "constant", type.Constant);
		shader->Set(prefix + "linear", type.Linear);
		shader->Set(prefix + "quadratic", type.Quadratic);

		break;
	case LightType::Type::DIRECTIONAL:
		prefix = "u_DirectionalLights[" + std::to_string(directionalIdx) + "].";
		directionalIdx++;

		GetUpDirection();
		shader->Set(prefix + "direction", glm::vec3(GetWorldTransform()[2]));

		break;
	}

	shader->Set(prefix + "position", glm::vec3(GetWorldTransform()[3]));
	shader->Set(prefix + "color", type.Color);
}

std::string Light::ToString() const
{
	std::stringstream ss;

	ss << Super::ToString();

	return ss.str();
}

std::string LightType::ToString() const
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
