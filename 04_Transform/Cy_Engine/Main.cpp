//加载纹理库
//地址 https://github.com/nothings/stb/blob/master/stb_image.h    
#define STB_IMAGE_IMPLEMENTATION
#include "Resource/stb_image.h"
//glm数学库<0.99版本以下>
//地址：https://glm.g-truc.net/0.9.8/index.html https://github.com/g-truc/glm/tags
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
//打印库
#include <iostream>
//OpenGl的语言库
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader\\Public\\Shader.h"


//assimp 模型库
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos); //鼠标角度函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);//
void processInput(GLFWwindow* window);//键盘输入函数

//--------------------------------------------------参数设置--------------------------------------------------
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
char vertexShaderPath[] = "D:/z_Work/-OpenGLRenderer-/04_Transform/Cy_Engine/File/VertexShader.glsl";
char fragmentShaderPath[] = "D:/z_Work/-OpenGLRenderer-/04_Transform/Cy_Engine/File/FragmentShader.glsl";
char tex01path[] = "D:/z_Work/-OpenGLRenderer-/04_Transform/Cy_Engine/Content/D.tga";
char tex02path[] = "D:/z_Work/-OpenGLRenderer-/04_Transform/Cy_Engine/Content/S0043_GLOW_1.tga";
//Shader Properties
float Shadervalue = 1.0f;
//相机相关数据
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);//相机位置
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);//相机方向
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);//相机速度

//
/*
计算鼠标距上一帧的偏移量。
把偏移量添加到摄像机的俯仰角和偏航角中。
对偏航角和俯仰角进行最大和最小值的限制。
计算方向向量。
*/
bool firstMouse = true;
float yaw = -90.0f;	//俯仰角 yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;//偏航角
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;//镜头透视角度


void processInput(GLFWwindow* window)
{
	//GLFW_RELEASE释放状态
	//GLFW_PRESS 按住状态
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		//设置OpenGL窗口是否需要关闭
		glfwSetWindowShouldClose(window, true);
	}
	float cameraSpeed = 0.55f* deltaTime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

float CtrlShaderValue(GLFWwindow* window , float value)
{
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS &&value<1.0f)
	{
		value += 0.1f;
		printf("ShaderAlpha提升0.1");
		return value;

	}
	else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && value > 0.0f)
	{
		value -= 0.1f;
		printf("ShaderAlpha降低0.1");
		return value;
	}
	return value;
}


int main()
{	
#pragma region 界面初始化
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

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, &mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

#pragma region 着色器加载
	//声明 并构造shader类sd
	C_Shader* V_Shader = new C_Shader(vertexShaderPath, fragmentShaderPath);
#pragma endregion

#pragma region 模型网格加载
	float cube[] = {
	//记录一个正方体 18个三角面按顶点逆时针绘制
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	//VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	//VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	//VAO绑定VBO信息 
	//参数1：定义顶点属性的ID 可以自定义
	//参数2：顶点属性的数据个数
	//参数3：type：顶点描述数据的类型，这里position数组中的数据全部为float，所以是GL_FLOAT
	//参数4：normalized：是否需要显卡帮忙把数据归一化到-1到+1区间，这里不需要，所以设置GL_FALSE
	//参数5：stride：一个顶点占有的总的字节数，比如为两个float，所以是sizeof(float)*2
	//参数6：pointer：当前指针指向的vertex内部的偏离字节数，可以唯一的标识顶点某个属性的偏移量 每次计算偏移多少数据
	//顶点 Location = 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);	
	glEnableVertexAttribArray(0);
	//UV Location = 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
#pragma endregion

#pragma region 纹理加载模块
	//纹理加载模块
	unsigned int tex01;
	glGenTextures(1, &tex01);
	glBindTexture(GL_TEXTURE_2D, tex01);
	//设置平铺属性
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//纹理利库 std——image功能 定义格式 宽高 通道数
	int width, height, texchannel;
	stbi_set_flip_vertically_on_load(true);// tell stb_image.h to flip loaded texture's on the y-axis. 加载时设置垂直翻转
	unsigned char* data = stbi_load(tex01path, &width, &height, &texchannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	unsigned int tex02;
	glGenTextures(1, &tex02);
	glBindTexture(GL_TEXTURE_2D, tex02);
	//设置平铺属性
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);// tell stb_image.h to flip loaded texture's on the y-axis. 加载时设置垂直翻转
	data = stbi_load(tex02path, &width, &height, &texchannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	//告诉每个采样器的opengl它属于哪个纹理单元（只需要做一次）
	V_Shader->use();
	//glUniform1i(glGetUniformLocation(testShader->ID, "Texture01"), 0);
	V_Shader->SetInt("Texture01", 0);
	V_Shader->SetInt("Texture02", 1);
	
#pragma endregion

#pragma region 线框渲染
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//开启现况渲染功能
#pragma endregion


#pragma region 渲染循环
	//--------------------------------------------------渲染阶段--------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);//处理输入指令按F键退出窗口

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);


		glActiveTexture(GL_TEXTURE0);//激活纹理单元(最多16张 你懂)
		glBindTexture(GL_TEXTURE_2D, tex01);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex02);
		Shadervalue = CtrlShaderValue(window, Shadervalue);//实现按T G 键交替闪烁功能


		//计算delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//----------------------------------------M 矩阵-----------------------------------------------------------------
		//参数1：矩阵的shader Location 位置
		//参数2：发送多少矩阵
		//参数3：询问我们是否希望对我们的矩阵进行转置(Transpose) OpenGL开发者通常使用一种内部矩阵布局，叫做列主序(Column-major Ordering)布局
		//参数4：矩阵数据
		glm::mat4 trans = glm:: mat4(1.0f);
		//trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 1.0f));
		//trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
		unsigned int transformloc = glGetUniformLocation(V_Shader->ID, "M");
		glUniformMatrix4fv(transformloc, 1, GL_FALSE, glm::value_ptr(trans));
		//----------------------------------------V 矩阵-----------------------------------------------------------------
		glm::mat4 view;
		//float radius = 10.0f;
		//float camx = sin(glfwGetTime()) * radius;
		//float camz = cos(glfwGetTime()) * radius;
		//相机空间的 lookat 参数1是相机坐标 参数2 目标 参数三 向上UP向量
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		V_Shader->setMat4("V", view);
		//transformloc = glGetUniformLocation(V_Shader->ID, "V");
		//glUniformMatrix4fv(transformloc, 1, GL_FALSE, &view[0][0]);
		//----------------------------------------P 矩阵-----------------------------------------------------------------
		glm::mat4 Proj;
		Proj = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//transformloc = glGetUniformLocation(V_Shader->ID, "P_Matrix");
		//glUniformMatrix4fv(transformloc, 2, GL_FALSE, glm::value_ptr(Proj));
		//V_Shader->SetMat4("P_Matrix", Proj);
		glUniformMatrix4fv(glGetUniformLocation(V_Shader->ID, "P"),1, GL_FALSE, &Proj[0][0]);

		V_Shader->SetFloat("lerpvalue", Shadervalue);
		V_Shader->use();
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glDrawArrays(GL_TRIANGLES,0,36);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
#pragma endregion

#pragma region 退出程序
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(V_Shader->ID);
	glfwTerminate();// glfw: terminate, clearing all previously allocated GLFW resources.
	return 0;
#pragma endregion

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}