#version 130

struct BaseLight
{
    vec3 color;
    float intensity;
};

struct Attenuation
{
    float constant;
    float linear;
    float exponent;
};

struct PointLight
{
    BaseLight base;
    Attenuation atten;
	vec3 positionCamSpace;
    float range;
};

in vec3 out_CameraSpacePos;
in vec3 out_Normal;
in float out_illum;
in vec4 out_Color;

//Because gl_FragColor is no longer used.
out vec4 fragmentColor;

uniform PointLight pointLight;

vec4 calcLight(BaseLight base, vec3 direction, vec3 normal)
{
    float diffuseFactor = dot(normal, -direction);
    
    vec4 diffuseColor = vec4(0,0,0,0);
    vec4 specularColor = vec4(0,0,0,0);
    
    if(diffuseFactor > 0)
    {
        diffuseColor = vec4(base.color, 1.0) * base.intensity * diffuseFactor;
        
        vec3 directionToEye = normalize(out_CameraSpacePos);
        vec3 reflectDirection = normalize(reflect(direction, normal));
    }
    
    return diffuseColor;
}

vec4 calcPointLight(PointLight pointLight, vec3 normal)
{
    vec3 lightDirection = out_CameraSpacePos - pointLight.positionCamSpace;
    float distanceToPoint = length(lightDirection);
    
    if(distanceToPoint > pointLight.range)
        return vec4(0,0,0,0);
    
    lightDirection = normalize(lightDirection);
    
    vec4 color = calcLight(pointLight.base, lightDirection, normal);
    
    float attenuation = pointLight.atten.constant +
                         pointLight.atten.linear * distanceToPoint +
                         pointLight.atten.exponent * distanceToPoint * distanceToPoint +
                         0.0001;
                         
    return color ;// attenuation;
}

void main(void) 
{
	if(out_illum == 0.0f)
	{
		vec3 lightDirection = out_CameraSpacePos - pointLight.positionCamSpace;
		float distanceToPoint = length(lightDirection);
		float diffuseFactor = dot(out_Normal, -lightDirection);
	
		fragmentColor = out_Color * (vec4(pointLight.base.color, 1.0) * pointLight.base.intensity * diffuseFactor);
	}
	else
	{
		fragmentColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}