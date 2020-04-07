#version 330 core

uniform Projection
{
	mat4 uin_Projection;
};

uniform View
{
	mat4 uin_View;
};

in vec3 vsin_Position;

void main()
{
	gl_Position = uin_Projection * uin_View * vec4(vsin_Position, 1);
}
