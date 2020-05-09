#type vertex
#version 430
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Normal;
layout(location = 2) in vec2 a_Tangent;
layout(location = 3) in float a_Bitangent;
layout(location = 4) in float a_TexCoord;

uniform mat4 u_MVP;

out vec3 v_Normal;

void main()
{
	gl_Position = u_MVP * vec4(a_Position, 1.0);	
	v_Normal = a_Normal;
}

#type fragment
#version 430

layout(location = 0) out vec4 color;

in vec3 v_Normal;

uniform sampler2D u_Textures[16];

void main()
{
	color = vec4((v_Normal * 0.5 + 0.5), 1.0);
}