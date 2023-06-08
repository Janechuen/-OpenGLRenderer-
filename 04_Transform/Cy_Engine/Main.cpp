//���������
//��ַ https://github.com/nothings/stb/blob/master/stb_image.h    
#define STB_IMAGE_IMPLEMENTATION
#include "Resource/stb_image.h"
//glm��ѧ��<0.99�汾����>
//��ַ��https://glm.g-truc.net/0.9.8/index.html https://github.com/g-truc/glm/tags
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


//assimp ģ�Ϳ�
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos); //���ǶȺ���
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);//
void processInput(GLFWwindow* window);//�������뺯��

//--------------------------------------------------��������--------------------------------------------------
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
char vertexShaderPath[] = "D:/z_Work/-OpenGLRenderer-/04_Transform/Cy_Engine/File/VertexShader.glsl";
char fragmentShaderPath[] = "D:/z_Work/-OpenGLRenderer-/04_Transform/Cy_Engine/File/FragmentShader.glsl";
char tex01path[] = "D:/z_Work/-OpenGLRenderer-/04_Transform/Cy_Engine/Content/D.tga";
char tex02path[] = "D:/z_Work/-OpenGLRenderer-/04_Transform/Cy_Engine/Content/S0043_GLOW_1.tga";
//Shader Properties
float Shadervalue = 1.0f;
//����������
float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);//���λ��
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);//�������
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);//����ٶ�

//
/*
����������һ֡��ƫ������
��ƫ������ӵ�������ĸ����Ǻ�ƫ�����С�
��ƫ���Ǻ͸����ǽ���������Сֵ�����ơ�
���㷽��������
*/
bool firstMouse = true;
float yaw = -90.0f;	//������ yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;//ƫ����
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;//��ͷ͸�ӽǶ�


void processInput(GLFWwindow* window)
{
	//GLFW_RELEASE�ͷ�״̬
	//GLFW_PRESS ��ס״̬
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		//����OpenGL�����Ƿ���Ҫ�ر�
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

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, &mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

#pragma region ��ɫ������
	//���� ������shader��sd
	C_Shader* V_Shader = new C_Shader(vertexShaderPath, fragmentShaderPath);
#pragma endregion

#pragma region ģ���������
	float cube[] = {
	//��¼һ�������� 18�������水������ʱ�����
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
	
	//VAO��VBO��Ϣ 
	//����1�����嶥�����Ե�ID �����Զ���
	//����2���������Ե����ݸ���
	//����3��type�������������ݵ����ͣ�����position�����е�����ȫ��Ϊfloat��������GL_FLOAT
	//����4��normalized���Ƿ���Ҫ�Կ���æ�����ݹ�һ����-1��+1���䣬���ﲻ��Ҫ����������GL_FALSE
	//����5��stride��һ������ռ�е��ܵ��ֽ���������Ϊ����float��������sizeof(float)*2
	//����6��pointer����ǰָ��ָ���vertex�ڲ���ƫ���ֽ���������Ψһ�ı�ʶ����ĳ�����Ե�ƫ���� ÿ�μ���ƫ�ƶ�������
	//���� Location = 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);	
	glEnableVertexAttribArray(0);
	//UV Location = 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
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


#pragma region ��Ⱦѭ��
	//--------------------------------------------------��Ⱦ�׶�--------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);//��������ָ�F���˳�����

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);


		glActiveTexture(GL_TEXTURE0);//��������Ԫ(���16�� �㶮)
		glBindTexture(GL_TEXTURE_2D, tex01);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex02);
		Shadervalue = CtrlShaderValue(window, Shadervalue);//ʵ�ְ�T G ��������˸����


		//����delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//----------------------------------------M ����-----------------------------------------------------------------
		//����1�������shader Location λ��
		//����2�����Ͷ��پ���
		//����3��ѯ�������Ƿ�ϣ�������ǵľ������ת��(Transpose) OpenGL������ͨ��ʹ��һ���ڲ����󲼾֣�����������(Column-major Ordering)����
		//����4����������
		glm::mat4 trans = glm:: mat4(1.0f);
		//trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 1.0f));
		//trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
		unsigned int transformloc = glGetUniformLocation(V_Shader->ID, "M");
		glUniformMatrix4fv(transformloc, 1, GL_FALSE, glm::value_ptr(trans));
		//----------------------------------------V ����-----------------------------------------------------------------
		glm::mat4 view;
		//float radius = 10.0f;
		//float camx = sin(glfwGetTime()) * radius;
		//float camz = cos(glfwGetTime()) * radius;
		//����ռ�� lookat ����1��������� ����2 Ŀ�� ������ ����UP����
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		V_Shader->setMat4("V", view);
		//transformloc = glGetUniformLocation(V_Shader->ID, "V");
		//glUniformMatrix4fv(transformloc, 1, GL_FALSE, &view[0][0]);
		//----------------------------------------P ����-----------------------------------------------------------------
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

#pragma region �˳�����
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