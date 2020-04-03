// Basic Texture Shader

#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexID;
layout(location = 4) in float a_TexScale;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexID;
out float v_TexScale;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexID = a_TexID;
	v_TexScale = a_TexScale;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexID;
in float v_TexScale;

uniform sampler2D u_Textures[16];

void main()
{
	color = texture(u_Textures[int(v_TexID)], v_TexCoord * v_TexScale) * v_Color;
}