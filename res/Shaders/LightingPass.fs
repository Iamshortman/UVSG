#version 130

in vec2 out_TexCoord;

//Because gl_FragColor is no longer used.
out vec4 fragmentColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;

void main(void) 
{
	fragmentColor = texture(gColor, out_TexCoord);
}
