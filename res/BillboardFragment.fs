#version 130

in vec3 out_FragPos;

uniform vec3 starColor;

//Because gl_FragColor is no longer used.
out vec4 FragmentColor;

void main(void) 
{
	float dist = length(out_FragPos) * 3.0;
	float brightness = (1.0 / (dist * dist) - 0.1) * 0.7;
	
	FragmentColor = vec4(starColor * brightness, 0.8f);
}