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

#define SIZE_OF(X, Y) int(X.size() * sizeof(Y))

#ifdef DEBUG
	#define GL(x)																								\
		x;																										\
		{																										\
			GLenum error = glGetError();																		\
			if (error != GL_NO_ERROR)																			\
				JS_ERROR("OpenGL Error ({}): {}		{}:{}", std::to_string(error), #x, __FILE__, __LINE__);		\
		}
#else
	#define GL(x) x
#endif

#include "Core/Tracers.h"

#include "Math/glm.h"
