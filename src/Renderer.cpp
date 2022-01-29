#include "Renderer.h"
#include <iostream>

void GlClearError(){
	while (glGetError() != GL_NO_ERROR);
}

void _GlGetError(const char *function, const char *file, int line) {
	while(GLenum errorCode = glGetError()){
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION"; break;
       /* case GL_STACK_OVERFLOW:
            error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:
            error = "STACK_UNDERFLOW"; break;*/
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << "[Opengl Error] (" << error << "):\nFunction: " << function <<"\nFile: "<< file << "\nLine:" << line << std::endl;
	}
}

int Renderer::width = 1600;
int Renderer::height = 900;

float MapRange(float from_x1, float from_x2, float to_x1, float to_x2, float x) {
    return (to_x2 - to_x1) / (from_x2 - from_x1) * (x - from_x1) + to_x1;
}

void scrollCallBack(GLFWwindow *window, double xoffset, double yoffset) {
    UserContext* context = (UserContext*)glfwGetWindowUserPointer(window);

	if(yoffset != 0){
		double mouseX, mouseY;

		glfwGetCursorPos(window, &mouseX, &mouseY);
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);

        float cx = MapRange(0, (float)width, context->rectMin.x, context->rectMax.x, (float)mouseX);
        float cy = MapRange(0, (float)height, context->rectMin.y, context->rectMax.y, (float)height - (float)mouseY);

		context->rectMin.x -= cx;
        context->rectMax.x -= cx;
        context->rectMin.y -= cy;
        context->rectMax.y -= cy;

        float factor = yoffset > 0 ? 0.9f : 1.1f;

        context->rectMin.x *= factor;
        context->rectMax.x *= factor;
        context->rectMin.y *= factor;
        context->rectMax.y *= factor;
        
        context->rectMin.x += cx;
        context->rectMax.x += cx;
        context->rectMin.y += cy;
        context->rectMax.y += cy;
	}
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    UserContext* context = (UserContext*)glfwGetWindowUserPointer(window);
	Renderer *renderer = context->renderer;
    renderer->width = width;
    renderer->height = height;
	context->renderer = renderer;
	glfwSetWindowUserPointer(window, context);
}





/*------------------------------------- RENDERER METHODS -----------------------------------------*/

void Renderer::clear(unsigned int bits) const{
    glClear(bits);
}

void Renderer::clear(float r, float g, float b, float a, unsigned int bits) const{
    glClearColor(r, g, b, a);
    glClear(bits);
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const{
    shader.bind();
    va.bind();
    ib.bind();

    glCheckError(glDrawElements(GL_TRIANGLES, ib.count, GL_UNSIGNED_INT, NULL));
}

void Renderer::drawfan(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.bind();
    va.bind();
    ib.bind();

    glCheckError(glDrawElements(GL_TRIANGLE_FAN, ib.count, GL_UNSIGNED_INT, NULL));

}
void Renderer::draw(const VertexArray& va, const Shader& shader, unsigned int vertexCount) const{
    va.bind();
    shader.bind();

    glCheckError(glDrawArrays(GL_TRIANGLES, 0, vertexCount));
}

void Renderer::processKeyboardInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
    UserContext* context = (UserContext*)glfwGetWindowUserPointer(window);


	// std::cout << "FPS->" << 1 / (deltaTime) << std::endl;


	float divideFactor = 50.0f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		float moveFactor = (context->rectMax.y - context->rectMin.y) / divideFactor;
		context->rectMin.y += moveFactor;
		context->rectMax.y += moveFactor;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		float moveFactor = (context->rectMax.x - context->rectMin.x) / divideFactor;
		context->rectMin.x -= moveFactor;
		context->rectMax.x -= moveFactor;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		float moveFactor = (context->rectMax.y - context->rectMin.y) / divideFactor;
		context->rectMin.y -= moveFactor;
		context->rectMax.y -= moveFactor;
    }
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		float moveFactor = (context->rectMax.x - context->rectMin.x) / divideFactor;
		context->rectMin.x += moveFactor;
		context->rectMax.x += moveFactor;
    }
	glfwSetWindowUserPointer(window, context);
}
