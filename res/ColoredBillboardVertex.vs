#version 130

in vec3 in_Position;
in vec3 in_Color; 

out vec3 out_Color;
out vec3 out_FragPos;

uniform vec3 tempColor;
uniform vec3 scale;
uniform mat4 MVP;

void main(void) 
{	
	gl_Position = MVP * vec4(in_Position * scale, 1.0f);
	out_Color = tempColor;
	out_FragPos = in_Position;
}
