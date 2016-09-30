#version 130

in vec3 out_Normal;
in vec2 out_TexCoord;
in vec3 out_WorldPos;

uniform vec3 ambientLight;
uniform sampler2D texture1;

//GBuffer output
out vec4 gColor;
out vec3 gPosition;
out vec3 gNormal;

void main(void) 
{
	//fragmentColor = vec4(abs(out_Normal), 1.0f);
	gColor = texture(texture1, out_TexCoord) * vec4(ambientLight, 1.0f);
	//gPosition = out_WorldPos;
	gPosition = (out_Normal + 1.0f) / 2.0f;
	gNormal = (out_Normal + 1.0f) / 2.0f;
}
