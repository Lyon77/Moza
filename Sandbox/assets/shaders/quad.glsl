#type vertex
#version 430
			
layout(location = 0) in vec3 a_Position;
layout(location = 4) in float a_TexCoord;

uniform mat4 u_InverseVP;

out vec3 v_Position;

void main()
{
	gl_Position = vec4(a_Position, 1.0);	
	v_Position = (u_InverseVP * vec4(a_Position, 1.0)).xyz;
}

#type fragment
#version 430

layout(location = 0) out vec4 color;

uniform samplerCube u_Texture;

in vec3 v_Position;

void main()
{
	color = textureLod(u_Texture, v_Position, 0.0);
}