#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

float Triangle03[]
{
	-0.0f,-0.5f,0.0f,
	0.9f,-0.5f,0.0f,
	0.45f,0.5f,0.0f,
};

void Mesh_VAO_Load(unsigned int vaomeshnum,unsigned int vaoindexmesh,unsigned int vbomeshnum, unsigned int  vboindexmesh)
{
	//VAO
	glGenVertexArrays(vaomeshnum, &vaoindexmesh);
	//VBO
	glGenBuffers(vbomeshnum, &vboindexmesh);
	for (unsigned int i =0 ; i< vaomeshnum; i++)
	{
		glBindVertexArray(vaoindexmesh[&i]);
		glBindBuffer(GL_ARRAY_BUFFER, vboindexmesh[&i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle03), Triangle03, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
}

void printf_Hello()
{
	std::cout << "AAAAAAAAA" << std::endl;
	//printf("引用成功");
}

