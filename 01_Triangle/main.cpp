#include <iostream>
//OpenGL Library
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader/Public/S_TriangleShader.h"
#include "Model/Public/MeshLoad.h"

//--------------------------------------------------第一套着色器--------------------------------------------------
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 vertexColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"vertexColor = aColor;\n"
"}\n\0";


const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 vertexColor;\n"
"void main()\n"
"{\n"

"FragColor = vec4(vertexColor,1.0);\n"
"}\n\0";

void processInput(GLFWwindow* window)
{
	//GLFW_RELEASE释放状态
	//GLFW_PRESS 按住状态
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		//设置OpenGL窗口是否需要关闭
		glfwSetWindowShouldClose(window, true);
	}
}



//常量：屏幕分辨率 
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
//函数：按键输入
void PressInput(GLFWwindow* window)
{
	if (glfwGetKey(window,GLFW_KEY_F)==GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{

	float Triangle01[]
	{
		-0.0f,-0.5f,0.0f,
		-0.9f,-0.5f,0.0f,
		-0.45f,0.5f,0.0f,
	};

	//--------------------------------------------------初始化交界面--------------------------------------------------
	//我们先尝试打印一下 但是我们需要先引用一个库 iostream C++的打印叫做printf();
	//printf("Hello UnderWorld");
	//glfwInit 初始化画面
	glfwInit();
	//使用OpenGL版本这是主版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//使用OpenGL版本这是次版本 所以是3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//ProFile  Core代表可编程流水线 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello OpenGL", NULL, NULL);
	//std::count << "成功" << std::endl;
	printf("成功");
	//window空了 做退出操作
	if (window == NULL)
	{
		//std::count << "失败" << std::endl;
		printf("失败了兄弟");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//Iint GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW失败了兄弟");
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);



#pragma region 着色器
	//--------------------------------------------------第一套顶点着色器阶段--------------------------------------------------
	//顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//--------------------------------------------------第一套片段着色器阶段--------------------------------------------------
	//片段着色器
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//编译二进制
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//--------------------------------------------------第一套着色器程序阶段--------------------------------------------------
	//着色器程序
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::SHADERPROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//VS 和PS绑shnum, unsigned int  vboindexm定到着色器程序上后就没用了删掉
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//--------------------------------------------------练习2三角形数据--------------------------------------------------
	//相比之前只输入一组三维点 我们再加一组三维顶点颜色
	float triangle01[]
	{
		//顶点信息         颜色信息
		 0.5f,-0.5f,0.0f,  1.0f,0.0f,0.0f,
		-0.5f,-0.5f,0.0f,  0.0f,1.0f,0.0f,
		 0.0f,0.5f,0.0f,  0.0f,0.0f,1.0f
	};

	unsigned int VBO;
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//第一个VBO VAO数据
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle01), triangle01, GL_STATIC_DRAW);
	//绑定VBO信息 参数1：为第一个顶点信息 隔三个为一个VBO数据 从第几个开始计算
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));// Vertex attributes stay the same
	glEnableVertexAttribArray(1);


	//--------------------------------------------------渲染阶段--------------------------------------------------
	//渲染循环开始
	while (!glfwWindowShouldClose(window))
	{

		//处理输入指令按F键退出窗口
		processInput(window);
		//处理渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//--------------------------------------------------绘制三角形--------------------------------------------------
		//使用着色器程序
		glUseProgram(shaderProgram);
		//绘制第一个VAO
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//检查调用事件
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteProgram(testShader->ID);
	glfwTerminate();
	return 0;

#pragma endregion
}

