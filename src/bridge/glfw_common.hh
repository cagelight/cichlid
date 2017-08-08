#pragma once

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace cichlid::glfw {
	
	extern bool initialized;
	extern GLFWwindow * win;
}

