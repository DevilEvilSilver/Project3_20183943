#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform	mat4 u_ViewProjection;
uniform	mat4 u_World;

out vec2 v_TexCoord;

void main()
{
	gl_Position = u_ViewProjection * u_World * vec4(a_Position, 1.0);
	v_TexCoord = a_TexCoord;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	color = vec4(1.0, 1.0, 1.0, 1.0);
}