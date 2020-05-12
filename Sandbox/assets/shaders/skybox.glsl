#type vertex
#version 430
layout(location = 0) in vec3 a_Position;

out vec3 v_TexCoords;

uniform mat4 u_VP;

void main()
{
	vec4 pos = u_VP * vec4(a_Position, 1.0);
	gl_Position = pos.xyww;
	v_TexCoords = a_Position;
};

#type fragment
#version 430

out vec4 color;

in vec3 v_TexCoords;

uniform samplerCube u_Texture;

void main()
{
	vec3 envColor = texture(u_Texture, v_TexCoords).rgb;
	color = vec4(envColor, 1.0);
	color = vec4(1.0);
}