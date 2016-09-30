#version 130

in vec3 out_Normal;
in vec4 out_Color;
in vec3 out_Ambient;
in float out_IllumValue;
in vec3 out_WorldPos;

//GBuffer output
out vec4 gColor;
out vec3 gPosition;
out vec3 gNormal;

void main(void) 
{
	gColor = out_Color * vec4(out_Ambient, 1.0f);
	//gColor = vec4((out_Normal + 1.0f) / 2.0f, 1.0f);
	//gPosition = out_WorldPos;
	gPosition = (out_Normal + 1.0f) / 2.0f;
	gNormal = (out_Normal + 1.0f) / 2.0f;
}