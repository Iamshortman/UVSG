#version 130

struct Material
{
	vec3 diffuse_Color;
	float alpha_Value;
	int illum; 
};

in vec3 in_Position;
in vec3 in_Normal;
in float in_Material; 

out vec3 out_Normal;
out vec4 out_Color;
out vec3 out_Ambient;

uniform mat4 MVP;
uniform mat3 normalMatrix; 
uniform mat4 localOffset = mat4(1.0);
uniform vec3 ambientLight;
 
uniform Material materials[16]; 

void main(void) 
{	
	vec4 offsetPosition = localOffset * vec4(in_Position, 1.0f);
	gl_Position = MVP * offsetPosition;
	out_Normal = normalMatrix * in_Normal;
	
	int i = int(in_Material);
	out_Color = vec4(materials[i].diffuse_Color, materials[i].alpha_Value);
	if(materials[i].illum == 0)
	{
		out_Ambient = vec3(1.0f, 1.0f, 1.0f);
	}
	else
	{
		out_Ambient = ambientLight;
	}
}
