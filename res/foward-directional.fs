#version 130

struct DirectionalLight
{
 	vec3 color;
	float intensity;
	vec3 direction;
};

in vec3 out_Normal;
in float out_illum;
in vec4 out_Color;

//Because gl_FragColor is no longer used.
out vec4 fragmentColor;

uniform DirectionalLight directionalLight;

void main(void) 
{
	if(out_illum == 0.0f)
	{
		float diffuseFactor = dot(out_Normal, -directionalLight.direction);
		fragmentColor = out_Color * (vec4(directionalLight.color, 1.0) * directionalLight.intensity * diffuseFactor);
	}
	else
	{
		fragmentColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}

}