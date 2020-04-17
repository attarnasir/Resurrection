#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int N_CURVE_PTS = 10; 
float controlPts[] = { 1.0f, 1.0f, 0.0f, 4.0f, 3.0f, 0.0f, 5.0f, 1.0f, 0.0f, 8.0, 2.0f, 0.0f };

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

void getVertices_bez3DeCasteljau(unsigned int N_CURVE_PTS, float* vertices) {
	float bx[4][3];
	float by[4][3];
	float bz[4][3];
	float b0x, b1x, b2x, b3x, b0y, b1y, b2y, b3y, b0z, b1z, b2z, b3z;
	float t = 0;
	b0x = *vertices;
	b0y = *++vertices;
	b0z = *++vertices;
	b1x = *++vertices;
	b1y = *++vertices;
	b1z = *++vertices;
	b2x = *++vertices;
	b2y = *++vertices;
	b2z = *++vertices;
	b3x = *++vertices;
	b3y = *++vertices;
	b3z = *++vertices;
	while (t < 1.1) {
		bx[1][0] = (1 - t)*b0x + t * b1x;
		by[1][0] = (1 - t)*b0y + t * b1y;
		bz[1][0] = (1 - t)*b0z + t * b1z;
		bx[1][1] = (1 - t)*b1x + t * b2x;
		by[1][1] = (1 - t)*b1y + t * b2y;
		bz[1][1] = (1 - t)*b1z + t * b2z;
		bx[1][2] = (1 - t)*b2x + t * b3x;
		by[1][2] = (1 - t)*b2y + t * b3y;
		bz[1][2] = (1 - t)*b2z + t * b3z;

		bx[2][0] = (1 - t)*bx[1][0] + t * bx[1][1];
		by[2][0] = (1 - t)*by[1][0] + t * by[1][1];
		bz[2][0] = (1 - t)*bz[1][0] + t * bz[1][1];
		bx[2][1] = (1 - t)*bx[1][1] + t * bx[1][2];
		by[2][1] = (1 - t)*by[1][1] + t * by[1][2];
		bz[2][1] = (1 - t)*bz[1][1] + t * bz[1][2];

		bx[3][0] = (1 - t)*bx[2][0] + t * bx[2][1];
		by[3][0] = (1 - t)*by[2][0] + t * by[2][1];
		bz[3][0] = (1 - t)*bz[2][0] + t * bz[2][1];
		cout << bx[3][0] << " " << by[3][0] << " "<<bz[3][0]<<endl;
		t = t + 0.1;
	}
}
void getVertices_bez3Mat(unsigned int N_CURVE_PTS, float* vertices) {
	float b[3][4], a[4][4], c[4][1], mul[3][4];
	float temp = 0;
	float r = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			b[j][i] = *vertices;
			vertices++;
		}
	}

	a[0][0] = 1;
	a[0][1] = -3;
	a[0][2] = 3;
	a[0][3] = -1;
	a[1][0] = 0;
	a[1][1] = 3;
	a[1][2] = -6;
	a[1][3] = 3;
	a[2][0] = 0;
	a[2][1] = 0;
	a[2][2] = 3;
	a[2][3] = -3;
	a[3][0] = 0;
	a[3][1] = 0;
	a[3][2] = 0;
	a[3][3] = 1;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				mul[i][j] = b[i][k] * a[k][j];
				temp = temp + mul[i][j];
				if (k == 3) {
					mul[i][j] = temp;
					temp = 0;
				}

			}
		}
	}

	while (r < 1.1) {
		float bx = mul[0][0] + mul[0][1] * r + mul[0][2] * r*r + mul[0][3] * r*r*r;
		float by = mul[1][0] + mul[1][1] * r + mul[1][2] * r*r + mul[1][3] * r*r*r;
		float bz = mul[2][0] + mul[2][1] * r + mul[2][2] * r*r + mul[2][3] * r*r*r;
		cout << bx << " " << by <<" "<< bz << endl;
		r = r + 0.1;
	}

}

void getVertices_bez3Horner(unsigned int N_CURVE_PTS, float* vertices) {
	float a0x, a0y, a0z, a1x, a1y, a1z, a2x, a2y, a2z, a3x, a3y, a3z;
	float q = 0;
	a0x = *vertices;
	a0y = *++vertices;
	a0z = *++vertices;
	a1x = *++vertices;
	a1y = *++vertices;
	a1z = *++vertices;
	a2x = *++vertices;
	a2y = *++vertices;
	a2z = *++vertices;
	a3x = *++vertices;
	a3y = *++vertices;
	a3z = *++vertices;

	while (q < 1.1) {
		float s = 1 - q;
		float bx = ((s*a0x + 3 * q*a1x)*s + 3 * q*q*a2x)*s + q * q*q*a3x;
		float by = ((s*a0y + 3 * q*a1y)*s + 3 * q*q*a2y)*s + q * q*q*a3y;
		float bz = ((s*a0z + 3 * q*a1z)*s + 3 * q*q*a2z)*s + q * q*q*a3z;
		cout << bx << " " << by << " "<<bz<<endl;
		q = q + 0.1;
	}

}

int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//float var[8] = { 1,1,4,3,5,1,8,2 };
	unsigned int chunkSzBytes = N_CURVE_PTS * 3 * sizeof(float);
	float *vertices = (float*)malloc(chunkSzBytes);  
	vertices = controlPts;

	auto start = high_resolution_clock::now(); 

	getVertices_bez3DeCasteljau(N_CURVE_PTS, vertices);

	auto stop_1 = high_resolution_clock::now();

	getVertices_bez3Mat(N_CURVE_PTS, vertices);

	auto stop_2 = high_resolution_clock::now();

	getVertices_bez3Horner(N_CURVE_PTS, vertices);

	auto stop_3 = high_resolution_clock::now();

	auto duration_bez3DeCasteljau = duration_cast<microseconds>(stop_1 - start);
	std::cout << "de Casteljau time (ms)=  " << duration_bez3DeCasteljau.count() << std::endl;

	auto duration_bez3Mat = duration_cast<microseconds>(stop_2 - stop_1);
	std::cout << "bezMat time (ms)=  " << duration_bez3Mat.count() << std::endl;

	auto duration_bez3Horner = duration_cast<microseconds>(stop_3 - stop_2);
	std::cout << "Horner time (ms)=  " << duration_bez3Horner.count() << std::endl;

	
	GLfloat points[33]{
		1, 1, 0,
		1.844, 1.487, 0,
		2.592, 1.776, 0,
		3.268, 1.909, 0,
		3.896, 1.928, 0,
		4.5, 1.875, 0,
		5.104, 1.792, 0,
		5.732, 1.721, 0,
		6.408, 1.704, 0,
		7.156, 1.783, 0,
		8, 2, 0
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, chunkSzBytes, points, GL_STATIC_DRAW);



	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	

	
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP, 0, 4);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	free(vertices);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
