#version 130

struct DirectionalLight
{
 	vec3 color;
	float intensity;
	vec3 direction;
};
uniform DirectionalLight directionalLight;

in vec3 out_Normal;
in vec4 out_Color;
in vec3 out_Ambient;
in float out_IllumValue;
in vec3 out_WorldPos;

//Because gl_FragColor is no longer used.
out vec4 fragmentColor;

void main(void) 
{
	if(out_IllumValue != 0.0f)
	{
		float diffuseFactor = dot(out_Normal, -directionalLight.direction);
		fragmentColor = out_Color * (vec4(directionalLight.color, 1.0) * directionalLight.intensity * diffuseFactor);
	}
	else
	{
		fragmentColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}


