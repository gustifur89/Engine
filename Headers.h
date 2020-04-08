#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#define GLM_ENABLE_EXPERIMENTAL
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <thread>
#include <queue>
#include <stack>

#include <GL/glew.h>


#include <GLFW/glfw3.h>

#define GLM_FORCE_SWIZZLE 
#include <glm/glm.hpp>
#include <glm/gtx/vec_swizzle.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "PerlinNoise.hpp"

#define PI		3.14159265359
#define TO_RAD	0.01745329251
#define TO_DEG	57.2957795131

class Gen
{
public:
	static double random()
	{
		return ((rand() % 100000) / 100000.0);
	}

	static void checkGLError()
	{
		GLenum err;
		while ((err = glGetError())) {
			std::cout << err << "\n";
		}

	}
};
