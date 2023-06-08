#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 uv;
out vec3 vertexColor;
out vec2 Texcoord01;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
void main()
{
Texcoord01 = uv;
gl_Position = P*V*M*vec4(aPos, 1.0);
//gl_Position = M*vec4(aPos,1.0f);
}