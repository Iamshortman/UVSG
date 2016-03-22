#version 130

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord; 

out vec3 out_Normal;
out vec2 out_TexCoord;

uniform mat4 MVP;
uniform mat3 normalMatrix; 
uniform mat4 localOffset = mat4(1.0); 
 
void main(void) 
{	
	vec4 offsetPosition = localOffset * vec4(in_Position, 1.0f);
	gl_Position = MVP * offsetPosition;
	out_Normal = normalMatrix * in_Normal;
	out_TexCoord = in_TexCoord;
}
