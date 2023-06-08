#pragma once
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
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
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}


private:
	void checkCompileErrors(unsigned int shader, std::string type);
};

