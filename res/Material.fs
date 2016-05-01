#version 130

in vec3 out_Normal;
in vec4 out_Color;
in vec3 out_Ambient;
in float out_IllumValue;
in vec3 out_WorldPos;

//Because gl_FragColor is no longer used.
out vec4 fragmentColor;

uniform float alphaValue = 1.0f;

void main(void) 
{
	//fragmentColor = vec4(abs(out_Normal), out_Color.w);
	fragmentColor = out_Color * vec4(out_Ambient, alphaValue);
}