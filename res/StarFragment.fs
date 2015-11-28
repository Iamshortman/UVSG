#version 130

in vec3 out_FragPos;

uniform vec3 starColor;

//Because gl_FragColor is no longer used.
out vec4 FragmentColor;

void main(void) 
{
	FragmentColor = vec4(starColor, 1.0f);
}
