#type vertex
#version 430

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_InverseVP;

out vec3 v_Position;
out vec3 v_TexCoord;

void main()
{
	vec4 position = vec4(a_Position.xy, 1.0, 1.0);
	gl_Position = position;

	v_Position = (u_InverseVP * position).xyz;
	v_TexCoord = vec3(a_TexCoord, 1.0);
}

#type fragment
#version 430

layout(location = 0) out vec4 color;

uniform samplerCube u_Texture;

in vec3 v_Position;
in vec3 v_TexCoord;

void main()
{
	color = textureLod(u_Texture, v_Position, 0.0);

	vec3 envColor = texture(u_Texture, v_TexCoord).rgb;
	color = vec4(envColor, 1.0);
}