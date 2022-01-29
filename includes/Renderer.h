#pragma once
#include <IndexBuffer.h>
#include <IndexBuffer.h>
#include <VertexArray.h>
#include <Shaders.h>
#include <GLFW/glfw3.h>
#include "Math/vec.h"


/*--------------------------------------- ERROR CHECKING FUNCTIONS ---------------------------------------*/
// clear the previous error
void GlClearError();

// get all the errors
void _GlGetError(const char *function, const char *file, int line);

#define glCheckError(x) GlClearError();\
	x;\
	_GlGetError(#x, __FILE__, __LINE__);



/*--------------------------------------- CALLBACK FUNCTIONS ---------------------------------------*/
void mouseCallback(GLFWwindow* window, double xPos, double yPos);

void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset);

void framebufferSizeCallback(GLFWwindow* window, int width, int height);


/*--------------------------------------- Renderer Class ---------------------------------------*/
struct Renderer {
	static int width, height;

	//methods
	void clear(unsigned int bits = GL_COLOR_BUFFER_BIT) const;
	void clear(float r, float g, float b, float a, unsigned int bits = GL_COLOR_BUFFER_BIT) const;

	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	void drawfan(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	void draw(const VertexArray& va, const Shader& shader, unsigned int vertexCount) const;

	void processKeyboardInput(GLFWwindow* window);
};


struct UserContext {
	Renderer *renderer;
	MathLib::vec2 rectMin;
	MathLib::vec2 rectMax;
};