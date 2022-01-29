#pragma once
#include <string>
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <Math/matrix.h>

struct Shader {
	std::string vertexSource;
	std::string fragmentSource;
	std::string vertexFilePath;
	std::string fragmentFilePath;
	unsigned int rendererID;
	std::map < std::string, int> uniformStorage;

	Shader(std::string _vertexFilePath, std::string _fragmemntFilePath);

	void parseShader();

	int compileShader(std::string shaderSource, unsigned int type) const;

	unsigned int CreateShaderProgram();

	void bind() const;

	void unbind() const;

	void setUniform(std::string name, float value);
	
	void setUniform(std::string name, int value);

	void setUniform(std::string name, float v0, float v1, float v2, float v3);
	
	void setUniform(std::string name, float v0, float v1, float v2);

	void setUniform(std::string name, float v0, float v1);

	void setUniform(std::string name, MathLib::vec3 vec);

	void setUniform(std::string name, MathLib::vec2 vec);

	void setUniform(std::string name, MathLib::mat4 transform);

	int GetUniformLocation(std::string& name);

	~Shader();
};