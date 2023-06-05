#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Renderer.h>
#include "Math/matrix.h"
#include "Math/vec.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

const int width = 1980, height = 1080;
double MouseX, MouseY;

//extern "C"
//{
//	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
//}


struct DataPoint {
	std::string column1;
	std::string column2;
};

std::vector<DataPoint> readball(const std::string& filename) {
	std::vector<DataPoint> data;
	std::ifstream file(filename);
	std::string line;
	DataPoint dataPoint;

	while (std::getline(file, line)) {


		std::stringstream ss(line);
		std::string item;

		// Read the first column
		// std::getline(ss, item, ',');
		// dataPoint.column1 = item;

		if (std::getline(ss, item, ',')) {
			dataPoint.column1 = std::string(item);
		}
		else {
			std::cout << "Error reading column 1 in line: " << line << std::endl;
			continue;
		}

		// // Read the second column
		if (std::getline(ss, item, ',')) {
			dataPoint.column2 = std::string(item);
		}
		else {
			std::cout << "Error reading column 2 in line: " << line << std::endl;
			continue;
		}
		// std::getline(ss, item, ',');
		// dataPoint.column2 = item;
		// std::cout<<dataPoint.column1<<" "<<dataPoint.column2<<"\n";

		// std::cout<<"pushed";
		data.push_back(dataPoint);
	}
	file.close();
	return data;
}



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
	Shader movingVor("./resources/shaders/vor1vs.glsl", "./resources/shaders/vfs1.glsl");
	Shader motion("./resources/shaders/VertexShader.glsl", "./resources/shaders/PixelShader.glsl");
	Shader test("./resources/shaders/vor3vs.glsl", "./resources/shaders/vfs3.glsl");
	Shader ball("./resources/shaders/playvs.glsl", "./resources/shaders/playfs.glsl");

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



	double mx = 2 * (MouseX / width) - 1.0;
	double my = -2 * (MouseY / height) - 1.0;

	shader.bind();
	glUniform1f(glGetUniformLocation(shader.rendererID,"SCR_HEI"),float(height));
	glUniform1f(glGetUniformLocation(shader.rendererID, "SCR_WID"), float(width));
	glUniform2fv(glGetUniformLocation(shader.rendererID, "mouse"), 1, MathLib::vec2(mx, my).value_ptr());
	shader.unbind();


	movingVor.bind();
	glUniform1f(glGetUniformLocation(movingVor.rendererID, "SCR_HEI"), float(height));
	glUniform1f(glGetUniformLocation(movingVor.rendererID, "SCR_WID"), float(width));
	glUniform1f(glGetUniformLocation(movingVor.rendererID, "time"), float(glfwGetTime()));
	movingVor.unbind();


	motion.bind();
	glUniform1f(glGetUniformLocation(motion.rendererID, "SCR_HEI"), float(height));
	glUniform1f(glGetUniformLocation(motion.rendererID, "SCR_WID"), float(width));
	glUniform2fv(glGetUniformLocation(shader.rendererID, "mouse"), 1, MathLib::vec2(mx, my).value_ptr());
	glUniform1fv(glGetUniformLocation(motion.rendererID, "movingP"), 1, MathLib::vec2(float(glfwGetTime()), float(glfwGetTime())).value_ptr());
	motion.unbind();

	test.bind();
	glUniform1f(glGetUniformLocation(test.rendererID, "SCR_HEI"), float(height));
	glUniform1f(glGetUniformLocation(test.rendererID, "SCR_WID"), float(width));
	glUniform2fv(glGetUniformLocation(test.rendererID, "mouse"), 1, MathLib::vec2(mx, my).value_ptr());
	test.unbind();

	ball.bind();
	glUniform1f(glGetUniformLocation(ball.rendererID, "SCR_HEI"), float(height));
	glUniform1f(glGetUniformLocation(ball.rendererID, "SCR_WID"), float(width));
	glUniform2fv(glGetUniformLocation(ball.rendererID, "mouse"), 1, MathLib::vec2(mx, my).value_ptr());
	ball.unbind();
	
	//ball position data
	std::vector<DataPoint> balldata = readball("./resources/ballpos.csv");
	short int bcount = 0;

	int animated = 0;

	while (!glfwWindowShouldClose(window)){
		glClearColor(0.3f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//change shader based on key
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			animated = 2;

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			animated = 1;

		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
			animated = 0;

		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
			animated = 3;

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
			animated = 4;


		UserContext *context = (UserContext *)glfwGetWindowUserPointer(window);

		if (animated==0)
		{
			shader.bind();
			glUniform1f(glGetUniformLocation(shader.rendererID, "SCR_HEI"), float(height));
			glUniform1f(glGetUniformLocation(shader.rendererID, "SCR_WID"), float(width));
			mx = 2 * (MouseX / width) - 1.0;
			my = -(2 * (MouseY / height) - 1.0);
			glUniform2fv(glGetUniformLocation(shader.rendererID, "mouse"), 1, MathLib::vec2(mx,my).value_ptr());
			shader.unbind();
			renderer.draw(va, indexBuffer, shader);
		}
		

		else if (animated ==1)
		{
			movingVor.bind();
			glUniform1f(glGetUniformLocation(movingVor.rendererID, "SCR_HEI"), float(height));
			glUniform1f(glGetUniformLocation(movingVor.rendererID, "SCR_WID"), float(width));
			glUniform1f(glGetUniformLocation(movingVor.rendererID, "time"), float(glfwGetTime()));
			movingVor.unbind();
			renderer.draw(va, indexBuffer, movingVor);
		}

		else if (animated == 3){
			test.bind();
			glUniform1f(glGetUniformLocation(test.rendererID, "SCR_HEI"), float(height));
			glUniform1f(glGetUniformLocation(test.rendererID, "SCR_WID"), float(width));
			mx = 2 * (MouseX / width) - 1.0;
			my = -(2 * (MouseY / height) - 1.0);
			glUniform2fv(glGetUniformLocation(test.rendererID, "mouse"), 1, MathLib::vec2(mx, my).value_ptr());
			test.unbind();
			renderer.draw(va, indexBuffer, test);
		}

		else if (animated == 4)
		{
			ball.bind();
			glUniform1f(glGetUniformLocation(ball.rendererID, "SCR_HEI"), float(height));
			glUniform1f(glGetUniformLocation(ball.rendererID, "SCR_WID"), float(width));
			
			//reset if more than datasize reset
			//bcount = 0 ? bcount >= balldata.size() : bcount++;
			// the data is in resolution 3840 × 2160
			if (bcount < balldata.size())
			{
				mx = 2 * std::stod(balldata[bcount].column1) / 3840 - 1.0;
				my = 2 * std::stod(balldata[bcount].column2) / 2160 - 1.0;
				bcount++;
			}
			else
				bcount = 0;

			//std::cout << bcount << "\t" << balldata[bcount].column1 << "\t" << balldata[bcount].column2 << std::endl;
			std::cout << bcount << "\t" << mx << "\t" << my << std::endl;
			glUniform2fv(glGetUniformLocation(ball.rendererID, "mouse"), 1, MathLib::vec2(mx, my).value_ptr());
			ball.unbind();
			renderer.draw(va, indexBuffer, ball);
		}
		
		else if (animated==2)
		{
			motion.bind();
			glUniform1f(glGetUniformLocation(motion.rendererID, "SCR_HEI"), float(height));
			glUniform1f(glGetUniformLocation(motion.rendererID, "SCR_WID"), float(width));
			mx = 2 * (MouseX / width) - 1.0;
			my = -(2 * (MouseY / height) - 1.0);
			glUniform2fv(glGetUniformLocation(motion.rendererID, "mouse"), 1, MathLib::vec2(mx, my).value_ptr());
			glUniform2fv(glGetUniformLocation(motion.rendererID, "time"), 1, MathLib::vec2(float(glfwGetTime()), float(glfwGetTime()/1.618)).value_ptr());
			//std::cout << float(abs(sin(glfwGetTime()))) <<"\n";
			motion.unbind();
			renderer.draw(va, indexBuffer, motion);
		}


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

