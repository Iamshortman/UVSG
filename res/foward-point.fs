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
	vec3 positionWorld;
    float range;
};
uniform PointLight pointLight;

in vec3 out_Normal;
in vec4 out_Color;
in vec3 out_Ambient;
in float out_IllumValue;
in vec3 out_WorldPos;

//Because gl_FragColor is no longer used.
out vec4 fragmentColor;

vec4 CalcLight(BaseLight base, vec3 direction, vec3 normal, vec3 worldPos)
{
    float diffuseFactor = dot(normal, -direction);
    
    vec4 diffuseColor = vec4(0,0,0,0);
    vec4 specularColor = vec4(0,0,0,0);
    
    if(diffuseFactor > 0)
    {
        diffuseColor = vec4(base.color, 1.0) * base.intensity * diffuseFactor;
        
		vec3 C_eyePos = vec3(0,0,0); //EyePos is the origin
        vec3 directionToEye = normalize(C_eyePos - worldPos);
        //vec3 reflectDirection = normalize(reflect(direction, normal));
        vec3 halfDirection = normalize(directionToEye - direction);
        
        float specularFactor = dot(halfDirection, normal);
        //specularFactor = pow(specularFactor, specularPower);
		specularFactor = pow(specularFactor, 96.078431);
        
        if(specularFactor > 0)
        {
            //specularColor = vec4(base.color, 1.0) * specularIntensity * specularFactor;
			specularColor = vec4(base.color, 1.0) * 0.5f * specularFactor;
        }
    }
    
    return diffuseColor + specularColor;
}

vec4 CalcPointLight(PointLight pointLight0, vec3 normal, vec3 worldPos)
{
    vec3 lightDirection = worldPos - pointLight0.positionWorld;
    float distanceToPoint = length(lightDirection);
    
    if(distanceToPoint > pointLight0.range)
        return vec4(0,0,0,0);
    
    lightDirection = normalize(lightDirection);
    
    vec4 color = CalcLight(pointLight0.base, lightDirection, normal, worldPos);
    
    float attenuation = pointLight0.atten.constant +
                         pointLight0.atten.linear * distanceToPoint +
                         pointLight0.atten.exponent * distanceToPoint * distanceToPoint +
                         0.0001;
                         
    return color / attenuation;
}

void main(void) 
{
	if(out_IllumValue != 0.0f)
	{
	
		float distanceToPoint = distance(out_WorldPos, pointLight.positionWorld);
		if( distanceToPoint < pointLight.range)
		{
			fragmentColor = out_Color * CalcPointLight(pointLight, out_Normal, out_WorldPos);
		}
		else
		{
			fragmentColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}
	else
	{
		fragmentColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}