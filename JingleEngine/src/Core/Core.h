#pragma once

#define GL_GLEXT_PROTOTYPES

#include <glad.h>

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Binding.h"
#include "Logging.h"
#include "Reference.h"

#define SIZE_OF(X, Y) X.size() * sizeof(Y)
