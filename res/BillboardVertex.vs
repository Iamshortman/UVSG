#version 130

in vec3 in_Position;
in vec3 in_Normal;

out vec3 out_FragPos;

uniform mat4 MVP;
uniform mat3 normalMatrix;
uniform mat4 modelMatrix; 
uniform vec3 scale;  
  
void main(void) 
{	
	vec3 normal = normalMatrix * in_Normal;
	gl_Position = MVP * vec4( (in_Position) * scale, 1.0f);
	out_FragPos = in_Position;
}