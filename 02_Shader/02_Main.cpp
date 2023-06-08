//打印库
#include <iostream>
//OpenGl的语言库
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader\\Public\\_02_Shader.h"


//--------------------------------------------------参数设置--------------------------------------------------
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
char vertexShaderPath[] = "File/VertexShader.glsl";
char fragmentShaderPath[] = "File/FragmentShader.glsl";


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



int main()
{
	
#pragma region 界面设置
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


	//--------------------------------------------------创建渲染界面--------------------------------------------------
	//OpenGL Window Creater
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "My OpenGL Draw", NULL, NULL);
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

#pragma endregion

	


#pragma region 着色器
	//声明 并构造shader类
	_02_Shader* testShader = new _02_Shader(vertexShaderPath, fragmentShaderPath);
	
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
	while (!glfwWindowShouldClose(window))
	{
	
		//处理输入指令按F键退出窗口
		processInput(window);
		//处理渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//--------------------------------------------------绘制三角形--------------------------------------------------
		//使用着色器程序
		
		//glUseProgram(shaderProgram);
		//绘制第一个VAO
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		testShader->use();
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