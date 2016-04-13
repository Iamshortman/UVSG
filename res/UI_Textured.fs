#version 130

in vec2 out_TexCoord;

//Because gl_FragColor is no longer used.
out vec4 fragmentColor;

uniform sampler2D texture1;

void main(void) 
{
	//fragmentColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	fragmentColor = texture(texture1, out_TexCoord);
	//fragmentColor = vec4(out_TexCoord, 0.0f, 1.0f);
}
