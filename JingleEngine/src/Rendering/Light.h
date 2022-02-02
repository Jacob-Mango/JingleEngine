#pragma once

#include "../Core/Application.h"
#include "../Core/Config.h"
#include "../Core/Entity.h"
#include "../Core/Scene.h"

class Texture;
class Shader;

class Light;
class LightType : public EntityType
{
	ENTITY_TYPE(Light, Entity)

	typedef EntityType base;

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

	virtual std::string ToString() override;
};

class Light : public Entity
{
	friend class Scene;

	typedef Entity base;

	ENTITY(Light, Entity)

private:
	glm::vec3 m_TempLightPosition;
	Ref<Texture> m_Shadow;

public:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	void Process(Ref<Shader> shader, int& pointIdx, int& directionalIdx);

	virtual std::string ToString() override;
};
