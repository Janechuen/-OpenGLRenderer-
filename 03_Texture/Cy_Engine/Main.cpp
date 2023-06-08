//���������
//��ַ https://github.com/nothings/stb/blob/master/stb_image.h    
#define STB_IMAGE_IMPLEMENTATION
#include "Resource/stb_image.h"
//glm��ѧ��
//��ַ��https://glm.g-truc.net/0.9.8/index.html https://github.com/g-truc/glm/tags

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
//��ӡ��
#include <iostream>
//OpenGl�����Կ�
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader\\Public\\Shader.h"

//--------------------------------------------------��������--------------------------------------------------
unsigned int SCR_WIDTH = 1920;
unsigned int SCR_HEIGHT = 1080;
char vertexShaderPath[] = "D:/z_Work/-OpenGLRenderer-/03_Texture/Cy_Engine/File/VertexShader.glsl";
char fragmentShaderPath[] = "D:/z_Work/-OpenGLRenderer-/03_Texture/Cy_Engine/File/FragmentShader.glsl";
char tex01path[] = "D:/z_Work/-OpenGLRenderer-/03_Texture/Cy_Engine/Content/T_Noise01.tga";
char tex02path[] = "D:/z_Work/-OpenGLRenderer-/03_Texture/Cy_Engine/Content/sda.tga";

//Shader Properties
float Shadervalue = 1.0f;

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

float CtrlShaderValue(GLFWwindow* window , float value)
{
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS &&value<1.0f)
	{
		value += 0.1f;
		printf("ShaderAlpha����0.1");
		return value;

	}
	else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && value > 0.0f)
	{
		value -= 0.1f;
		printf("ShaderAlpha����0.1");
		return value;
	}
	return value;
}


int main()
{
	
#pragma region �����ʼ��
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

#pragma region ��ɫ������
	//���� ������shader��sd
	C_Shader* V_Shader = new C_Shader(vertexShaderPath, fragmentShaderPath);
#pragma endregion

#pragma region ģ���������
	float triangle01[]
	{
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
		 0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
	};
	unsigned int indices[] =
	{
		0,1,2,
		2,3,0
	};//EBO��¼�Ķ�������


	//VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle01), triangle01, GL_STATIC_DRAW);
	
	//VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	//EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//VAO��VBO��Ϣ 
	//����1�����嶥�����Ե�ID �����Զ���
	//����2���������Ե����ݸ���
	//����3��type�������������ݵ����ͣ�����position�����е�����ȫ��Ϊfloat��������GL_FLOAT
	//����4��normalized���Ƿ���Ҫ�Կ���æ�����ݹ�һ����-1��+1���䣬���ﲻ��Ҫ����������GL_FALSE
	//����5��stride��һ������ռ�е��ܵ��ֽ���������Ϊ����float��������sizeof(float)*2
	//����6��pointer����ǰָ��ָ���vertex�ڲ���ƫ���ֽ���������Ψһ�ı�ʶ����ĳ�����Ե�ƫ���� ÿ�μ���ƫ�ƶ�������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));// Vertex attributes stay the same
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));// Vertex attributes stay the same
	glEnableVertexAttribArray(2);
#pragma endregion

#pragma region �������ģ��
	//�������ģ��
	unsigned int tex01;
	glGenTextures(1, &tex01);
	glBindTexture(GL_TEXTURE_2D, tex01);
	//����ƽ������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//�������� std����image���� �����ʽ ��� ͨ����
	int width, height, texchannel;
	stbi_set_flip_vertically_on_load(true);// tell stb_image.h to flip loaded texture's on the y-axis. ����ʱ���ô�ֱ��ת
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
	//����ƽ������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);// tell stb_image.h to flip loaded texture's on the y-axis. ����ʱ���ô�ֱ��ת
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
	//����ÿ����������opengl�������ĸ�����Ԫ��ֻ��Ҫ��һ�Σ�
	V_Shader->use();
	//glUniform1i(glGetUniformLocation(testShader->ID, "Texture01"), 0);
	V_Shader->SetInt("Texture01", 0);
	V_Shader->SetInt("Texture02", 1);
	
#pragma endregion

#pragma region �߿���Ⱦ
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�����ֿ���Ⱦ����
#pragma endregion

#pragma region aa

	//vec = trans * vec;
	//std::cout << vec.x << vec.y << vec.z << std::endl;

#pragma region ��Ⱦѭ��
	//--------------------------------------------------��Ⱦ�׶�--------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);//��������ָ�F���˳�����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);//��������Ԫ(���16�� �㶮)
		glBindTexture(GL_TEXTURE_2D, tex01);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex02);
		Shadervalue = CtrlShaderValue(window, Shadervalue);//ʵ�ְ�T G ��������˸����

		
		//----------------------------------------M ����-----------------------------------------------------------------
		// 	glm::vec4 vec(0.0f, 0.0f, 0.0f, 1.0f);
		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(0.0f, -2.0f, -3.0f));
		trans = glm::scale(trans, glm::vec3(0.2f, 0.2f, 0.2f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0));
		//����1�������shader Location λ��
		//����2�����Ͷ��پ���
		//����3��ѯ�������Ƿ�ϣ�������ǵľ������ת��(Transpose) OpenGL������ͨ��ʹ��һ���ڲ����󲼾֣�����������(Column-major Ordering)����
		//����4����������
		unsigned int transformloc = glGetUniformLocation(V_Shader->ID, "M_Matrix");
		glUniformMatrix4fv(transformloc, 0, GL_FALSE, glm::value_ptr(trans));


		V_Shader->SetFloat("lerpvalue", Shadervalue);
		V_Shader->use();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
#pragma endregion

#pragma region �˳�����
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(V_Shader->ID);
	glfwTerminate();// glfw: terminate, clearing all previously allocated GLFW resources.
	return 0;
#pragma endregion

}