#version 130

in vec3 out_Normal;
in vec3 out_Color;

//Because gl_FragColor is no longer used.
out vec4 fragmentColor;

void main(void) 
{
	fragmentColor = vec4(out_Color, 1.0f);
}