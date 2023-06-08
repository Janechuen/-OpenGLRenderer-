#include "D:/z_Work/-OpenGLRenderer-/03_Texture/Cy_Engine/Shader/Public/Shader.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
//using namespace std; �������ͻ

C_Shader::C_Shader(const char* vertexPath, const char* fragmentPath)
{
	//���� �ļ����洢λ�� ���ڴ��ļ�
	std::ifstream vertexFile;
	std::ifstream fragmentFile;
	//string�� ���ڶ�ȡ file �ļ��е���Ϣ ���ļ������  string
	std::stringstream vertexSSteam;
	std::stringstream fragmentSSteam;

	//�򿪶�Ӧ�ļ�·���е��ļ�
	vertexFile.open(vertexPath);
	fragmentFile.open(fragmentPath);
	//��⻳��
	vertexFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);

	//�ж��Ƿ�򿪴��� û�д򿪻ᱨ��
	try
	{
		if (!vertexFile.is_open() || !fragmentFile.is_open())
		{
			throw std::exception("Open file error");
		}
		vertexSSteam << vertexFile.rdbuf();
		fragmentSSteam << fragmentFile.rdbuf();
		//��stringstream ���ݴ��� string
		vertexString = vertexSSteam.str();
		fragmentString = fragmentSSteam.str();
		//��string��ʽת��Ϊchar��ʽ
		vertexSource = vertexString.c_str();
		fragmentSource = fragmentString.c_str();

		//printf("���������������ɫ��GLSL��:\r\n%s\r\n", vertexSource);
		//printf("���������������ɫ��GLSL��:\r\n%s\r\n", fragmentSource);

		unsigned int vertex = 0;
		unsigned int frag = 0;
		int success;//����ֵ �����ɫ�������Ƿ�ɹ�
		char infolog[512];

		////--------------------------------------------------������ɫ���׶�--------------------------------------------------
		//����shaderһ��ID ��������
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexSource, NULL);
		//shader�Ὣԭ���� ����ɶ����� 
		glCompileShader(vertex);
		//����������Ƿ�ɹ�
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infolog);
			std::cout << "������ɫ��_�������\n" << infolog << std::endl;
		}


		//--------------------------------------------------Ƭ����ɫ���׶�--------------------------------------------------
		frag = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag, 1, &fragmentSource, NULL);
		//shader�Ὣԭ���� ����ɶ����� 
		glCompileShader(frag);
		//����������Ƿ�ɹ�
		glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infolog);
			std::cout << "������ɫ��_�������\n" << infolog << std::endl;
		}
		//--------------------------------------------------��ɫ������׶�--------------------------------------------------
		////��ɫ������
		ID = glCreateProgram();
		//��id��������
		glAttachShader(ID, vertex);
		glAttachShader(ID, frag);
		//���� LINK ������������Ϊ��������������������һ�� �����Ķ����ƴ��������ȷ�� ��Ȼ���޷�����
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infolog);
			std::cout << "ERROR::SHADER::SHADERPROGRAM::COMPILATION_FAILED\n" << infolog << std::endl;
		}
		//printf("������ɫ����ЧID: %d\r\n", vertex);
		//printf("������ɫ����ЧID: %d\r\n", frag);
		//printf("��ɫ��������ЧID: %d\r\n", ID);
		//�ɹ����Ӻ� ���� ������ɫ��û���� ɾ����
		glDeleteShader(vertex);
		glDeleteShader(frag);
	}

	catch (const std::exception& ex)
	{
		printf(ex.what());
	}
}



//Public
void C_Shader::use()
{
	glUseProgram(ID);

}
void C_Shader::SetBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void C_Shader::SetInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void C_Shader::SetFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

//Private
void checkCompileErrors(unsigned int shader, std::string type)
{
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
