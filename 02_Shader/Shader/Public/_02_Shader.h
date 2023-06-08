#pragma once
#include <string>
class _02_Shader
{
public:
	unsigned int ID; //Shader program ID 
	//存储正式的问文本信息
	std::string vertexString;
	std::string fragmentString;
	//只写入一次 提供给GLSL使用时最后Shader文本 OPenGL支持 char*格式
	const char* vertexSource;
	const char* fragmentSource;
	_02_Shader(const char* vertexPath, const char* fragmentPath);
	void use();
private:
};

