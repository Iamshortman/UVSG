#version 130

in vec3 out_Normal;
in vec4 out_Color;
in vec3 out_Ambient;
in float out_IllumValue;
in vec3 out_WorldPos;

//GBuffer output
out vec4 gColor;

void main(void) 
{
	gColor = out_Color * vec4(out_Ambient, 1.0f);
}