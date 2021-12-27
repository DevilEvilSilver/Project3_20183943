#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in ivec3 a_JointID;
layout(location = 4) in vec3 a_Weight;

uniform	mat4 u_ViewProjection;
uniform	mat4 u_World;
uniform mat4 u_JointTransform[];

out vec3 v_Normal;
out vec2 v_TexCoord;

void main()
{
	gl_Position = u_ViewProjection * u_World * vec4(a_Position, 1.0);
	v_TexCoord = a_TexCoord;
	v_Normal = a_Normal;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;

const vec3 lightDirection = vec3(0.0, 1.0, 1.0);
const vec2 lightBias = vec2(0.7, 0.6); //just indicates the balance between diffuse and ambient lighting

void main()
{
	vec4 diffuseColour = texture(u_Texture, v_TexCoord);		
	vec3 unitNormal = normalize(v_Normal);
	float diffuseLight = max(dot(-lightDirection, unitNormal), 0.0) * lightBias.x + lightBias.y;
	color = diffuseColour * diffuseLight;
}