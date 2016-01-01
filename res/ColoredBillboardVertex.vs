#version 130

in vec3 in_Position;
in vec3 in_Color; 

out vec3 out_Color;

uniform vec3 cameraPos;
uniform vec3 scale;
uniform mat4 projectionMatrix; 

void main(void) 
{	
	vec4 position = vec4(cameraPos * scale, 1.0f);
	gl_Position = projectionMatrix * position;
	out_Color = in_Color;
}
