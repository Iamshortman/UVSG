#version 130

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord; 

out vec3 out_Normal;
out vec2 out_TexCoord;

uniform mat4 MVP;
uniform mat3 normalMatrix; 
 
void main(void) 
{	
	gl_Position = MVP * vec4(in_Position, 1.0f);
	out_Normal = normalMatrix * in_Normal;
	out_TexCoord = in_TexCoord;
}
