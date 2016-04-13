#version 130

in vec3 in_Position;
in vec2 in_TexCoord; 

out vec2 out_TexCoord;
 
void main(void) 
{	
	gl_Position = vec4(in_Position * 0.9f, 1.0f);
	out_TexCoord = vec2(in_TexCoord.x, 1.0f - in_TexCoord.y);
}
