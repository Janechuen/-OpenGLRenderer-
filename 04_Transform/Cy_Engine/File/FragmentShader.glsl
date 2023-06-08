#version 330 core
//输出像素
out vec4 FragColor;
//输入顶点色
in vec3 vertexColor;
//输入UV
in vec2 Texcoord01;
//输入纹理
uniform sampler2D Texture01;
uniform sampler2D Texture02;
uniform float lerpvalue;
void main()
{
vec3 col = vec3(vertexColor);
vec3 tex1 = texture(Texture01,Texcoord01).xyz;
vec3 tex2 = texture(Texture02,Texcoord01).xyz;
FragColor = vec4(mix(vec4(tex1,1.0f),vec4(tex2,1.0f),lerpvalue));
}