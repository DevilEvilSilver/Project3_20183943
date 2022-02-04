#type vertex
#version 330 core

const int MAX_JOINTS = 100;//max joints allowed in a skeleton
const int MAX_WEIGHTS = 3;//max number of joints that can affect a vertex

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in ivec3 a_JointID;
layout(location = 4) in vec3 a_Weight;

uniform	mat4 u_ViewProjection;
uniform	mat4 u_World;
uniform mat4 u_JointTransform[MAX_JOINTS];

out vec3 v_Pos;
out vec3 v_Normal;
out vec2 v_TexCoord;

void main()
{
	vec4 totalLocalPos = vec4(0.0);
	vec4 totalNormal = vec4(0.0);
	
	for(int i = 0; i < MAX_WEIGHTS; i++)
	{
		mat4 jointTransform = u_JointTransform[a_JointID[i]];
		vec4 posePosition = jointTransform * vec4(a_Position, 1.0);
		totalLocalPos += posePosition * a_Weight[i];
		
		vec4 worldNormal = jointTransform * vec4(a_Normal, 0.0);
		totalNormal += worldNormal * a_Weight[i];
	}

	gl_Position = u_ViewProjection * u_World * totalLocalPos;
	v_TexCoord = a_TexCoord;
	v_Normal = totalNormal.xyz;
	v_Pos = vec3(a_Position);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Pos;
in vec3 v_Normal;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;

const vec3 lightPos = vec3(0.0, -3.0, 10.0);
const vec3 lightColor = vec3(1.0);
const vec3 ambient = vec3(0.2);

void main()
{
	vec4 objColor = texture(u_Texture, v_TexCoord);		
	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize(lightPos - v_Pos);
	vec3 diffuse = max(dot(normal, lightDir), 0.0) * lightColor;

	color = vec4(vec3(objColor) * (diffuse + ambient), 1.0);
}