#version 130

in vec3 in_Position;
in vec3 in_Color; 
in vec3 in_Normal;

out vec3 out_Color; 
out vec3 out_Normal; 
out vec3 out_Light;

uniform mat4 MVP;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform vec3 DirectionLight;
 
void main(void) 
{	
	out_Light = DirectionLight;
	out_Color = in_Color;
	out_Normal = normalize(NormalMatrix * in_Normal);
	gl_Position = MVP * vec4(in_Position, 1.0f);
}
