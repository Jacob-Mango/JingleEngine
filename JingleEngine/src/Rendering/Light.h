#pragma once

#include "Core/Core.h"

#include "Scene/Scene.h"

class Texture;
class Shader;
class Scene;

class Light;
class LightType : public EntityType
{
	DEFINE_CLASS(LightType, EntityType);

public:
	enum class Type
	{
		DIRECTIONAL,
		POINT
	} Type;

	glm::vec3 Color;

	float Constant;
	float Linear;
	float Quadratic;

public:
	LightType() {}

	virtual void Load(Config& config) override;

	virtual std::string ToString() const override;

};

class Light : public Entity
{
	DEFINE_CLASS(Light, Entity);

	friend Scene;

private:
	glm::vec3 m_TempLightPosition;
	Ref<Texture> m_Shadow;

public:
	Light() {}

	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	void Process(Ref<Shader> shader, int& pointIdx, int& directionalIdx);

	virtual std::string ToString() const override;
};
