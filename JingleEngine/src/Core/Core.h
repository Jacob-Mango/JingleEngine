#pragma once

#define GL_GLEXT_PROTOTYPES

#include "Platform/OpenGL/glad.h"

#include <imgui.h>

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Logging.h"
#include <JingleScript.h>

#define SIZE_OF(X, Y) int(X.size() * sizeof(Y))
