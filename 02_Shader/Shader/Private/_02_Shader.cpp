#include "A:\\PROJECT_WORKS\\VS_PROJECT\\Cy_OpenGLEngine\\02_Shader\\Shader\\Public\\_02_Shader.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//using namespace std; 会引起冲突

_02_Shader::_02_Shader(const char* vertexPath, const char* fragmentPath)
{
	//声明 文件流存储位置 用于打开文件
	std::ifstream vertexFile;
	std::ifstream fragmentFile;
	//string流 用于读取 file 文件中的信息 将文件传输给  string
	std::stringstream vertexSSteam;
	std::stringstream fragmentSSteam;

	//打开对应文件路径中的文件
	vertexFile.open(vertexPath);
	fragmentFile.open(fragmentPath);
	//检测怀挡
	vertexFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);

	//判断是否打开处理 没有打开会报错
	try
	{
		if (!vertexFile.is_open()|| !fragmentFile.is_open())
		{
			throw std::exception("Open file error");
		}
		vertexSSteam << vertexFile.rdbuf();
		fragmentSSteam << fragmentFile.rdbuf();
		//将stringstream 数据传给 string
		vertexString = vertexSSteam.str();
		fragmentString = fragmentSSteam.str();
		//将string格式转换为char格式
		vertexSource = vertexString.c_str();
		fragmentSource = fragmentString.c_str();
		printf("测试输出【顶点着色器GLSL】:\r\n%s\r\n", vertexSource);
		printf("测试输出【像素着色器GLSL】:\r\n%s\r\n", fragmentSource);
	}
	catch (const std:: exception& ex)
	{
		printf(ex.what());
	}

	unsigned int vertex=0;
	unsigned int frag=0;
	int success;//返回值 检查着色器编译是否成功
	char infolog[512];

	////--------------------------------------------------顶点着色器阶段--------------------------------------------------
	//给该shader一个ID 方便领认
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex,1,&vertexSource,NULL);
	//shader会将原代码 翻译成二进制 
	glCompileShader(vertex);
	//检查编译情况是否成功
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infolog);
		std::cout << "顶点着色器_编译错误\n" << infolog << std::endl;
	}
	


	//--------------------------------------------------片段着色器阶段--------------------------------------------------
	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragmentSource, NULL);
	//shader会将原代码 翻译成二进制 
	glCompileShader(frag);
	//检查编译情况是否成功
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infolog);
		std::cout << "像素着色器_编译错误\n" << infolog << std::endl;
	}
	//--------------------------------------------------着色器程序阶段--------------------------------------------------
	////着色器程序
	ID = glCreateProgram();
	//往id附件属性
	glAttachShader(ID, vertex);
	glAttachShader(ID, frag);
	//链接 LINK 这样操作是因为这又两串代码有序链接一起 出来的二进制代码才是正确的 不然将无法分清
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::SHADERPROGRAM::COMPILATION_FAILED\n" << infolog << std::endl;
	}
	printf("顶点着色器生效ID: %d\r\n", vertex);
	printf("像素着色器生效ID: %d\r\n", frag);
	printf("着色器程序生效ID: %d\r\n", ID);
	//成功链接后 顶点 像素着色器没用了 删除掉
	glDeleteShader(vertex);
	glDeleteShader(frag);

}
void _02_Shader::use()
{
	glUseProgram(ID);
	
}


