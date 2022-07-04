#pragma once

#include <JingleScript.h>

#include <string>
#include <sstream>

#include <glm/glm.hpp>

#include <glm/detail/qualifier.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#define JS_LINK_GLM_TYPE(glmType)																								\
	struct js_##glmType : glm::glmType																							\
	{																															\
		DEFINE_NAMED_STRUCTURE(glmType, js_##glmType);																			\
		using glm::glmType::glmType;																							\
	public:																														\
		static glm::glmType FromString(std::string value);																		\
		std::string ToString() const;																							\
	};																															\
	namespace JingleScript																										\
	{																															\
	template<>																													\
	Type* ObjectTypeImpl<glm::glmType>();																						\
	template<>																													\
	Type* ObjectInitTypeImpl<glm::glmType>();																					\
	}																															\
	template <typename T>																										\
	struct fmt::formatter<T, std::enable_if_t<std::is_base_of<glm::glmType, T>::value, char>> : fmt::formatter<std::string>		\
	{																															\
		template <typename FormatCtx>																							\
		auto format(const T& value, FormatCtx& ctx)																				\
		{																														\
			return fmt::formatter<std::string>::format(glm::to_string(value), ctx);												\
		}																														\
	}

#define BEGIN_GLM_LINK(glmType)																									\
	namespace JingleScript																										\
	{																															\
	template<>																													\
	Type* ObjectTypeImpl<glm::glmType>()																						\
	{																															\
		return js_##glmType::StaticType();																						\
	}																															\
	template<>																													\
	Type* ObjectInitTypeImpl<glm::glmType>()																					\
	{																															\
		return js_##glmType::StaticInitType();																					\
	}																															\
	}																															\
	BEGIN_STRUCTURE_LINK(js_##glmType);																							\
		LINK_CONSTRUCTOR();																										\
		LINK_FUNCTION(FromString);																								\
		LINK_METHOD(ToString);
	

JS_LINK_GLM_TYPE(vec1);
JS_LINK_GLM_TYPE(dvec1);
JS_LINK_GLM_TYPE(vec2);
JS_LINK_GLM_TYPE(dvec2);
JS_LINK_GLM_TYPE(vec3);
JS_LINK_GLM_TYPE(dvec3);
JS_LINK_GLM_TYPE(vec4);
JS_LINK_GLM_TYPE(dvec4);

JS_LINK_GLM_TYPE(mat3);
JS_LINK_GLM_TYPE(dmat3);
JS_LINK_GLM_TYPE(mat4);
JS_LINK_GLM_TYPE(dmat4);

JS_LINK_GLM_TYPE(quat);
JS_LINK_GLM_TYPE(dquat);
