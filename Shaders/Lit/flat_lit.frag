#version 330 core

struct DirectionalLight
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutOff;
	float outerCutOff;
	
    float constant;
    float linear;
    float quadratic;
};


in vec3 tFragPos;
in vec3 tNormal;
in vec3 tObjScale;
in vec3 tObjNormal;

uniform vec3 flatColor;
uniform float specularStrength;

uniform vec3 viewPos;


uniform DirectionalLight dirLight;
uniform PointLight pointLights[64];
uniform int nbPointLights;
uniform SpotLight spotLights[32];
uniform int nbSpotLights;


out vec4 FragColor;


vec3 ComputeAllLights(vec3 normal, vec3 viewDir);
vec3 ComputeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 ComputePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);
vec3 ComputeSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos);

void main()
{
	//  properties
	vec3 norm = normalize(tNormal);
	vec3 viewDir = normalize(viewPos - tFragPos);

	//  compute lights
	vec3 lightingResult = ComputeAllLights(norm, viewDir);

	//  result
	FragColor = vec4(lightingResult, 1.0f);
}


vec3 ComputeAllLights(vec3 normal, vec3 viewDir)
{
	//  directional light
	vec3 result = ComputeDirectionalLight(dirLight, normal, viewDir);

	//  point lights
	for(int i = 0; i < nbPointLights; i++)
	{
		result += ComputePointLight(pointLights[i], normal, viewDir, tFragPos);
	}

	//  spot lights
	for(int i = 0; i < nbSpotLights; i++)
	{
		result += ComputeSpotLight(spotLights[i], normal, viewDir, tFragPos);
	}

	return result;
}


vec3 ComputeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	//  ambient
	vec3 ambient = light.ambient * flatColor;

	//  diffuse
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * flatColor;

	//  specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
	vec3 specular = light.specular * spec * specularStrength;

	//  result
	vec3 result = ambient + diffuse + specular;
	return result;
}

vec3 ComputePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
	//  ambient
	vec3 ambient = light.ambient * flatColor;

	//  diffuse
	vec3 lightDir = normalize(light.position - tFragPos);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * flatColor;

	//  specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
	vec3 specular = light.specular * spec * specularStrength;

	//  attenuation
	float lightDist = length(light.position - tFragPos);
	float attenuation = 1.0f / (light.constant + light.linear * lightDist + light.quadratic * (lightDist * lightDist));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	//  result
	vec3 result = ambient + diffuse + specular;
	return result;
}

vec3 ComputeSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
	//  ambient
	vec3 ambient = light.ambient * flatColor;

	//  diffuse
	vec3 lightDir = normalize(light.position - tFragPos);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * flatColor;

	//  specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
	vec3 specular = light.specular * spec * specularStrength;
    
    //  spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    //  attenuation
    float lightDist = length(light.position - tFragPos);
    float attenuation = 1.0 / (light.constant + light.linear * lightDist + light.quadratic * (lightDist * lightDist));    
    ambient *= attenuation; 
    diffuse *= attenuation;
    specular *= attenuation;   
        
	//  result
    vec3 result = ambient + diffuse + specular;
	return result;
}