#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec3 a_Normal;

out vec4 v_Color;
out vec3 v_Normal;
out vec4 v_CurrentPos;
out vec4 v_LightPos;
uniform mat4 u_MVP;
uniform vec4 u_LightPos;

void main()
{
	gl_Position = u_MVP * position;
	v_Color = a_Color;
	v_Normal = a_Normal;
	v_CurrentPos = gl_Position;
	v_LightPos = u_LightPos;
};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec3 v_Normal;
in vec4 v_LightPos;
in vec4 v_CurrentPos;

//uniform vec4 u_Color;

void main()
{	
	float ambient = 0.50f;

	// diffuse lighting
	vec3 normal = normalize(v_Normal);

	vec3 lightDirection = normalize(v_LightPos - v_CurrentPos).xyz;

	float diffuse = max(normal.x*lightDirection.x + normal.y * lightDirection.y+ normal.z * lightDirection.z, 0.0f);

	color = v_Color * (0.0f,1.0f,0.0f, 1.0f) * (diffuse + ambient);
	color = vec4(color.xyz, 1.0f);
};