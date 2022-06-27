#pragma once

#define GL_GLEXT_PROTOTYPES

#include <JingleScript.h>

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/wglew.h>
//#include <GL/glxew.h>

#include <imgui.h>

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define SIZE_OF(X, Y) int(X.size() * sizeof(Y))

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <glm/detail/qualifier.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#define GLM_LOGGING(glmType)																									\
	template <typename T>																										\
	struct fmt::formatter<T, std::enable_if_t<std::is_base_of<glm::glmType, T>::value, char>> : fmt::formatter<std::string>		\
	{																															\
		template <typename FormatCtx>																							\
		auto format(const T& value, FormatCtx& ctx)																				\
		{																														\
			return fmt::formatter<std::string>::format(glm::to_string(value), ctx);												\
		}																														\
	}

GLM_LOGGING(vec2);
GLM_LOGGING(dvec2);
GLM_LOGGING(vec3);
GLM_LOGGING(dvec3);
GLM_LOGGING(vec4);
GLM_LOGGING(dvec4);
GLM_LOGGING(quat);
GLM_LOGGING(dquat);
GLM_LOGGING(mat3);
GLM_LOGGING(dmat3);
GLM_LOGGING(mat4);
GLM_LOGGING(dmat4);

#ifdef DEBUG
#define GL(x)																						\
	x;																								\
	{																								\
		GLenum error = glGetError();																\
		if (error != GL_NO_ERROR)																	\
			std::cerr << "[OpenGL Error] (" << std::to_string(error) << "): " << #x << std::endl	\
					  << "\t" << __FILE__ << ":" << __LINE__ << std::endl;							\
	}

#define OUT_LINE(x)																					\
	{																								\
		std::cout << "[OUTPUT] " << #x << std::endl													\
					  << "\t" << __FILE__ << ":" << __LINE__ << std::endl;							\
	}

#else
#define GL(x) x

#define OUT_LINE(x)
#endif

#include "Core/Tracers.h"
