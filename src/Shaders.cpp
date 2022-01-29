#include "Shaders.h"


Shader::Shader(std::string _vertexFilePath, std::string _fragmentFilePath):vertexFilePath(_vertexFilePath), fragmentFilePath(_fragmentFilePath) {
	parseShader();

	rendererID = CreateShaderProgram();
}

void Shader::parseShader(){
	std::ifstream stream;
	stream.open(vertexFilePath);

	std::string line;
	std::stringstream ss;

	while (std::getline(stream, line)) {
		ss << line << "\n";
	}
	vertexSource = ss.str();

	std::ifstream fragmentStream;
	std::stringstream ssFragment;

	fragmentStream.open(fragmentFilePath);
	while (std::getline(fragmentStream, line)) {
		ssFragment << line << "\n";
	}

	fragmentSource = ssFragment.str();
}



int Shader::compileShader(std::string shaderSource, unsigned int type) const {
	unsigned int id = glCreateShader(type);

	const char* src = shaderSource.c_str();
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "Failed to compile " << "File: " << (type == GL_VERTEX_SHADER ? vertexFilePath : fragmentFilePath) <<
			(type == GL_VERTEX_SHADER ? " (vertex" : " (fragment") << " shader)\n";
		std::cout << infoLog << std::endl;
		glDeleteShader(id);
		// exit(-1);
		return 0;
	}
	return id;
}

unsigned int Shader::CreateShaderProgram(){
	unsigned int shaderProgram = glCreateProgram();

	unsigned int vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
	unsigned int fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void Shader::bind() const{
	glUseProgram(rendererID);
}

void Shader::unbind() const{
	glUseProgram(0);
}

void Shader::setUniform(std::string name, float value){
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::setUniform(std::string name, int value){
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::setUniform(std::string name, float v0, float v1, float v2, float v3){
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniform(std::string name, float v0, float v1, float v2) {
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}


void Shader::setUniform(std::string name, MathLib::vec3 vec) {
	glUniform3fv(GetUniformLocation(name),1, vec.value_ptr());
}

void Shader::setUniform(std::string name, MathLib::vec2 vec) {
	glUniform2fv(GetUniformLocation(name),1, vec.value_ptr());
}


void Shader::setUniform(std::string name, MathLib::mat4 transform){
	glUniformMatrix4fv(GetUniformLocation(name), 1, false, transform.value_ptr());
}

int Shader::GetUniformLocation(std::string& name){
	if (uniformStorage.find(name) != uniformStorage.end()) {
		return uniformStorage[name];
	}
	int location = glGetUniformLocation(rendererID, name.c_str());
	if (location == -1)
		std::cerr << "Uniform " << name << " does not exist." << " File path: " << vertexFilePath << "   " << fragmentFilePath << "\n";
	uniformStorage[name] = location;
	return location;
}

Shader::~Shader(){
	glDeleteProgram(rendererID);
}
