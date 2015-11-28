#version 130

in vec3 out_Normal;
in vec2 out_TexCoord;
in vec3 out_FragPos;

//Because gl_FragColor is no longer used.
out vec4 fragmentColor;

uniform sampler2D texture1;

uniform vec3 lightColor;
uniform vec3 lightPos; 
//uniform vec3 viewPos;

void main(void) 
{
	// Diffuse 
    vec3 norm = normalize(out_Normal);
    vec3 lightDir = normalize(lightPos - out_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
	
	diff = floor(diff * 4);
	diff /= 4;
	
	float distanceFromLight = abs( distance(lightPos, out_FragPos));
	
    vec3 diffuse = diff * lightColor;

	if(distanceFromLight > 10)
	{
		diffuse = vec3(0.1f, 0.1f, 0.1f);
	}
	
	vec3 result = diffuse * texture(texture1, out_TexCoord).xyz;
	fragmentColor = vec4(result, 1.0f);
}