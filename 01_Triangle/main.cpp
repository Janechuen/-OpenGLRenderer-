#include <iostream>
//OpenGL Library
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader/Public/S_TriangleShader.h"
#include "Model/Public/MeshLoad.h"

//--------------------------------------------------��һ����ɫ��--------------------------------------------------
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
	//GLFW_RELEASE�ͷ�״̬
	//GLFW_PRESS ��ס״̬
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		//����OpenGL�����Ƿ���Ҫ�ر�
		glfwSetWindowShouldClose(window, true);
	}
}



//��������Ļ�ֱ��� 
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
//��������������
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

	//--------------------------------------------------��ʼ��������--------------------------------------------------
	//�����ȳ��Դ�ӡһ�� ����������Ҫ������һ���� iostream C++�Ĵ�ӡ����printf();
	//printf("Hello UnderWorld");
	//glfwInit ��ʼ������
	glfwInit();
	//ʹ��OpenGL�汾�������汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//ʹ��OpenGL�汾���Ǵΰ汾 ������3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//ProFile  Core����ɱ����ˮ�� 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello OpenGL", NULL, NULL);
	//std::count << "�ɹ�" << std::endl;
	printf("�ɹ�");
	//window���� ���˳�����
	if (window == NULL)
	{
		//std::count << "ʧ��" << std::endl;
		printf("ʧ�����ֵ�");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//Iint GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		printf("GLEWʧ�����ֵ�");
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);



#pragma region ��ɫ��
	//--------------------------------------------------��һ�׶�����ɫ���׶�--------------------------------------------------
	//������ɫ��
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
	//--------------------------------------------------��һ��Ƭ����ɫ���׶�--------------------------------------------------
	//Ƭ����ɫ��
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//���������
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//--------------------------------------------------��һ����ɫ������׶�--------------------------------------------------
	//��ɫ������
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
	//VS ��PS��shnum, unsigned int  vboindexm������ɫ�������Ϻ��û����ɾ��
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//--------------------------------------------------��ϰ2����������--------------------------------------------------
	//���֮ǰֻ����һ����ά�� �����ټ�һ����ά������ɫ
	float triangle01[]
	{
		//������Ϣ         ��ɫ��Ϣ
		 0.5f,-0.5f,0.0f,  1.0f,0.0f,0.0f,
		-0.5f,-0.5f,0.0f,  0.0f,1.0f,0.0f,
		 0.0f,0.5f,0.0f,  0.0f,0.0f,1.0f
	};

	unsigned int VBO;
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//��һ��VBO VAO����
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle01), triangle01, GL_STATIC_DRAW);
	//��VBO��Ϣ ����1��Ϊ��һ��������Ϣ ������Ϊһ��VBO���� �ӵڼ�����ʼ����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));// Vertex attributes stay the same
	glEnableVertexAttribArray(1);


	//--------------------------------------------------��Ⱦ�׶�--------------------------------------------------
	//��Ⱦѭ����ʼ
	while (!glfwWindowShouldClose(window))
	{

		//��������ָ�F���˳�����
		processInput(window);
		//������Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//--------------------------------------------------����������--------------------------------------------------
		//ʹ����ɫ������
		glUseProgram(shaderProgram);
		//���Ƶ�һ��VAO
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//�������¼�
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

