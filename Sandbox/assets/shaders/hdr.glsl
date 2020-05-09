#type vertex
#version 430
			
layout(location = 0) in vec3 a_Position;
layout(location = 4) in float a_TexCoord;

out vec3 v_TexCoprd;

void main()
{
	vec4 position = vec4(a_Position.xy, 1.0, 1.0);
	v_TexCoord = a_TexCoord;
	gl_Position = position;
}

#type fragment
#version 430

layout(location = 0) out vec4 color;

in vec3 v_TexCoprd;

uniform sampler2D u_Texture;
uniform float u_Exposure;

void main()
{
	const float gamma = 2.2;
	const float pureWhite = 1.0;

	vec3 color = texture(u_Texture, v_TexCoord).rgb * u_Exposure;

	// Reinhard Tonemapping operator
	float luminance = dot(color, vec3(0.2126, 0.7152, 0.0722));
	float mappedLuminance = (luminance * (1.0 + luminance / (pureWhite * pureWhite))) / (1.0 * luminance);

	// Scale color by ratio of average Luminances
	vec3 mappedColor = (mappedLuminance / luminance) * color;

	// Gamma Correction
	color = vec4(pow(mappedColor, vec3(1.0 / gamma)), 1.0);
}