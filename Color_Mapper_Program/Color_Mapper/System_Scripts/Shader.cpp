#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>

#include "Shader.h"

using namespace std;

Shader::Shader(string codeFilePath, GLenum shaderType)
{
	ifstream fileReader(codeFilePath);
	stringstream stringBuffer;

	if (fileReader.is_open())
	{
		stringBuffer << fileReader.rdbuf();
		string code = stringBuffer.str();
		const char* constCode = (char*) code.c_str();
		
		//create gl shader
		this->_id = glCreateShader(shaderType);
		glShaderSource(this->_id, 1, &constCode, NULL);

		//compile shader
		glCompileShader(this->_id);

		//check for errors
		int isSuccesful;
		char infoLog[512];
		glGetShaderiv(this->_id, GL_COMPILE_STATUS, &isSuccesful);

		if (!isSuccesful)
		{
			glGetShaderInfoLog(this->_id, 512, NULL, infoLog);
			cout << "ERROR: " + to_string(shaderType) + " shader compilation failed \n" << infoLog << "----End of error log----" << endl << endl;
		}

		fileReader.close();
	}
	else
	{
		cout << "Couldn't open " + codeFilePath + " shader file!" << endl;
		return;
	}
}
