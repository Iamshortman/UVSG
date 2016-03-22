#version 130

in vec3 out_Normal;
in vec2 out_TexCoord;

//Because gl_FragColor is no longer used.
out vec4 fragmentColor;

uniform sampler2D texture1;

void main(void) 
{
	//fragmentColor = vec4(abs(out_Normal), 1.0f);
	fragmentColor = texture(texture1, out_TexCoord);
}
