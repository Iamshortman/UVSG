#version 130

in vec3 in_Position;
in vec3 in_Color; 
in vec3 in_Normal;

out vec3 out_Color; 

uniform mat4 MVP;
 
void main(void) 
{	
	out_Color = in_Color;
	gl_Position = MVP * vec4(in_Position, 1.0f);
}
