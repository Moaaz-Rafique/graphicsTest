#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 a_Color;
out vec4 v_Color;

void main()
{
	gl_Position = position;
	v_Color = a_Color;
};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;

//uniform vec4 u_Color;

void main()
{
	color = v_Color;
};