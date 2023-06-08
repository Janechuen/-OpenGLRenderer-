//��ӡ��
#include <iostream>
//OpenGl�����Կ�
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader\\Public\\_02_Shader.h"


//--------------------------------------------------��������--------------------------------------------------
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
char vertexShaderPath[] = "File/VertexShader.glsl";
char fragmentShaderPath[] = "File/FragmentShader.glsl";


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



int main()
{
	
#pragma region ��������
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


	//--------------------------------------------------������Ⱦ����--------------------------------------------------
	//OpenGL Window Creater
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "My OpenGL Draw", NULL, NULL);
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

#pragma endregion

	


#pragma region ��ɫ��
	//���� ������shader��
	_02_Shader* testShader = new _02_Shader(vertexShaderPath, fragmentShaderPath);
	
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
	while (!glfwWindowShouldClose(window))
	{
	
		//��������ָ�F���˳�����
		processInput(window);
		//������Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//--------------------------------------------------����������--------------------------------------------------
		//ʹ����ɫ������
		
		//glUseProgram(shaderProgram);
		//���Ƶ�һ��VAO
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		testShader->use();
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