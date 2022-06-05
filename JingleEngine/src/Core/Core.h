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
#include <glm/gtc/type_ptr.hpp>

std::ostream& operator<<(std::ostream& os, const glm::vec2& value);
std::ostream& operator<<(std::ostream& os, const glm::dvec2& value);
std::ostream& operator<<(std::ostream& os, const glm::vec3& value);
std::ostream& operator<<(std::ostream& os, const glm::dvec3& value);
std::ostream& operator<<(std::ostream& os, const glm::vec4& value);
std::ostream& operator<<(std::ostream& os, const glm::dvec4& value);
std::ostream& operator<<(std::ostream& os, const glm::mat4& value);
std::ostream& operator<<(std::ostream& os, const glm::mat3& value);
std::ostream& operator<<(std::ostream& os, const glm::quat& value);

#ifdef DEBUG
#define GL(x)                                                                                    \
	x;                                                                                           \
	{                                                                                            \
		GLenum error = glGetError();                                                             \
		if (error != GL_NO_ERROR)                                                                \
			std::cerr << "[OpenGL Error] (" << std::to_string(error) << "): " << #x << std::endl \
					  << "\t" << __FILE__ << ":" << __LINE__ << std::endl;                       \
	}

#define OUT_LINE(x)                                                                              \
	{                                                                                            \
		std::cout << "[OUTPUT] " << #x << std::endl                                              \
					  << "\t" << __FILE__ << ":" << __LINE__ << std::endl;                       \
	}

#else
#define GL(x) x

#define OUT_LINE(x)
#endif
