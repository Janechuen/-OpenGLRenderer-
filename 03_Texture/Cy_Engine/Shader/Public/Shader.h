#pragma once
#include <string>
class C_Shader
{
public:
	unsigned int ID; //Shader program ID 
	//存储正式的问文本信息
	std::string vertexString;
	std::string fragmentString;
	//只写入一次 提供给GLSL使用时最后Shader文本 OPenGL支持 char*格式
	const char* vertexSource;
	const char* fragmentSource;
	C_Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void SetBool(const std::string& name, bool value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
private:
};
