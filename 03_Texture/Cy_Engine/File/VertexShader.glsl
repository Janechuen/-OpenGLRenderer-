#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 uv;
out vec3 vertexColor;
out vec2 Texcoord01;

uniform mat4 transform;
void main()
{
Texcoord01 = uv;
vertexColor = aColor;
//gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0) * transform;
gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);


}