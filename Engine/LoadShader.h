#pragma once
#include "Headers.h"

class LoadShader
{
public:
	LoadShader();
	~LoadShader();

	static GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
};

