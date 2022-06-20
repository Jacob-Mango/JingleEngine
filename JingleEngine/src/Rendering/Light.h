#pragma once

#include "Core/Core.h"

#include "Scene/Scene.h"

class Texture;
class Shader;
class Scene;

class Light : public Entity
{
	DEFINE_CLASS(Light, Entity);

	friend Scene;

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
