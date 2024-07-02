#pragma once

#include <iostream>
#include <string>

#include <glad/glad.h>

using namespace std;

class Shader
{
public:
	Shader(string codeFilePath, GLenum shaderType);

	unsigned int GetId() { return this->_id; }

private:
	unsigned int _id;
};