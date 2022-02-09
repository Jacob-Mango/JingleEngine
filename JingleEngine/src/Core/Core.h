#pragma once

#define GL_GLEXT_PROTOTYPES

#include "../Platform/OpenGL/glad.h"

#include <imgui.h>

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Binding.h"
#include "Logging.h"
#include "Reference.h"
#include "Input.h"

#define SIZE_OF(X, Y) X.size() * sizeof(Y)
