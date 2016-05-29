#version 130

in vec3 in_Position;
in vec2 in_TexCoord; 

out vec2 out_TexCoord;

void main()
{
    gl_Position = vec4(in_Position, 1.0f);
    out_TexCoord = in_TexCoord;
}