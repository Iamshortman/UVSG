#version 130

in vec2 out_TexCoord;

//Because gl_FragColor is no longer used.
out vec4 fragmentColor;

uniform sampler2D gColor;
uniform sampler2D gPosition;
uniform sampler2D gNormal;

void main(void) 
{
	//fragmentColor = vec4(0, 0, 1, 1);
	//fragmentColor = vec4(out_TexCoord, 0, 0);
	//fragmentColor = vec4(1, 1, 1, 1) - texture(gColor, out_TexCoord);
	vec3 normal = ((texture(gNormal, out_TexCoord) * 2.0f) - 1.0f).xyz;
	
	fragmentColor = texture(gColor, out_TexCoord);
}
