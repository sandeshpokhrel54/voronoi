#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Renderer.h>
#include "Math/matrix.h"
#include "Math/vec.h"

const int width = 800, height = 600;
double MouseX, MouseY;

int main() {
	glfwInit();

	Renderer renderer;
	UserContext context;
	context.renderer = &renderer;

	renderer.width = width;
	renderer.height = height;
	float aspectRatio = float(width) / float(height);
	GLFWwindow* window = glfwCreateWindow(width, height, "Voronoi", NULL, NULL);
	
	//mouse button/cursor callback
	//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

	//open gl settings
	glfwWindowHint(GLFW_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetWindowUserPointer(window, &context);
	glfwSetScrollCallback(window, scrollCallBack);
	//glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	Shader shader("./resources/shaders/vor2vs.glsl", "./resources/shaders/vfs2.glsl");
	Shader movingVor("./resources/shaders/vor1vs.glsl", "./resources/shaders/vfs2.glsl");

	float vertices[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, 1.0f,
	};

	uint32_t indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		0, 2, 3    // second triangle
	};

	VertexBuffer vb(vertices, sizeof(vertices));
	IndexBuffer indexBuffer(indices, sizeof(indices));
	VertexBufferLayout vbLayout;
	vbLayout.push<float>(2);
	VertexArray va;
	va.addBuffer(vb, vbLayout);




	shader.bind();
	glUniform1f(glGetUniformLocation(shader.rendererID,"SCR_HEI"),float(height));
	glUniform1f(glGetUniformLocation(shader.rendererID, "SCR_WID"), float(width));
	double mx = 2 * (MouseX / width) - 1.0;
	double my = 2 * (MouseY / height) - 1.0;
	glUniform2fv(glGetUniformLocation(shader.rendererID, "mouse"), 1, MathLib::vec2(mx, my).value_ptr());
	shader.unbind();


	//movingVor.bind();
	//glUniform1f(glGetUniformLocation(movingVor.rendererID, "SCR_HEI"), float(height));
	//glUniform1f(glGetUniformLocation(movingVor.rendererID, "SCR_WID"), float(width));
	//movingVor.unbind();

	int cursor = 0;

	while (!glfwWindowShouldClose(window)){
		glClearColor(0.3f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		UserContext *context = (UserContext *)glfwGetWindowUserPointer(window);

		shader.bind();
		glUniform1f(glGetUniformLocation(shader.rendererID, "SCR_HEI"), float(height));
		glUniform1f(glGetUniformLocation(shader.rendererID, "SCR_WID"), float(width));
		mx = 2 * (MouseX / width) - 1.0;
		my = -(2 * (MouseY / height) - 1.0);
		glUniform2fv(glGetUniformLocation(shader.rendererID, "mouse"), 1, MathLib::vec2(mx,my).value_ptr());
		shader.unbind();

		renderer.draw(va, indexBuffer, shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
		renderer.processKeyboardInput(window);
	}
}


//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
//{
//	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//	{
//		glfwGetCursorPos(window, &MouseX, &MouseY);
//		//double mx = 2 * (MouseX / width) - 1.0;
//		//double my = -(2 * (MouseY / height) - 1.0);
//		//std::cout << "mx " << MouseX<<" "<<mx << " my " << MouseY<< " "<<my<<"\n";
//	}
//}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	glfwGetCursorPos(window, &MouseX, &MouseY);
}

