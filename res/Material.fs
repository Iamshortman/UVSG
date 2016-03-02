#version 130

in vec3 out_Normal;
in vec4 out_Color;
in vec3 out_Ambient;

//Because gl_FragColor is no longer used.
out vec4 fragmentColor;

void main(void) 
{
	//fragmentColor = vec4(abs(out_Normal), 1.0f);
	fragmentColor = out_Color;
}