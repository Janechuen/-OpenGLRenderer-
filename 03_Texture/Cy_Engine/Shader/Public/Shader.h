#pragma once
#include <string>
class C_Shader
{
public:
	unsigned int ID; //Shader program ID 
	//�洢��ʽ�����ı���Ϣ
	std::string vertexString;
	std::string fragmentString;
	//ֻд��һ�� �ṩ��GLSLʹ��ʱ���Shader�ı� OPenGL֧�� char*��ʽ
	const char* vertexSource;
	const char* fragmentSource;
	C_Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void SetBool(const std::string& name, bool value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
private:
};
