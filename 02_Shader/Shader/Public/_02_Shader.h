#pragma once
#include <string>
class _02_Shader
{
public:
	unsigned int ID; //Shader program ID 
	//�洢��ʽ�����ı���Ϣ
	std::string vertexString;
	std::string fragmentString;
	//ֻд��һ�� �ṩ��GLSLʹ��ʱ���Shader�ı� OPenGL֧�� char*��ʽ
	const char* vertexSource;
	const char* fragmentSource;
	_02_Shader(const char* vertexPath, const char* fragmentPath);
	void use();
private:
};

