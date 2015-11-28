#version 130

in vec3 in_Position;
in vec3 in_Color; 
in vec3 in_Normal;

out vec3 out_Color; 

uniform mat4 MVP;
uniform vec3 offset = vec3(0.0f, 0.0f, 0.0f);
uniform mat3 normalMatrix; 
 
void main(void) 
{	
	vec3 normal = normalMatrix * in_Normal;
	gl_Position = MVP * vec4( (in_Position + offset), 1.0f);
	
	out_Color = in_Color;
}
