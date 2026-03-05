#version 330 core

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_emissive1;
	float shininess;
};

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
in vec2 tTexCoord;
in vec3 tObjScale;
in vec3 tObjNormal;

uniform Material material;
uniform vec3 viewPos;

uniform bool beta_prevent_tex_scaling = false;
uniform float beta_tex_scaling_factor = 1;


uniform DirectionalLight dirLight;
uniform PointLight pointLights[64];
uniform int nbPointLights;
uniform SpotLight spotLights[32];
uniform int nbSpotLights;


out vec4 FragColor;


vec3 ComputeAllLights(vec3 normal, vec3 viewDir, vec2 texCoord);
vec3 ComputeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec2 texCoord);
vec3 ComputePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos, vec2 texCoord);
vec3 ComputeSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos, vec2 texCoord);

void main()
{
	//  properties
	vec3 norm = normalize(tNormal);
	vec3 viewDir = normalize(viewPos - tFragPos);

	//  recompute texCoord
	vec2 texCoord = tTexCoord;

	if(beta_prevent_tex_scaling == true) //  yeah this is extremly bad I know
	{
		texCoord.x *= tObjScale.x / beta_tex_scaling_factor;
		texCoord.y *= tObjScale.z / beta_tex_scaling_factor;
	}

	//  compute lights
	vec3 lightingResult = ComputeAllLights(norm, viewDir, texCoord);

	//  add emissive
	vec3 emissive = texture(material.texture_emissive1, texCoord).rgb;

	//  result
	FragColor = vec4(lightingResult + emissive, 1.0f);
}


vec3 ComputeAllLights(vec3 normal, vec3 viewDir, vec2 texCoord)
{
	//  directional light
	vec3 result = ComputeDirectionalLight(dirLight, normal, viewDir, texCoord);

	//  point lights
	for(int i = 0; i < nbPointLights; i++)
	{
		result += ComputePointLight(pointLights[i], normal, viewDir, tFragPos, texCoord);
	}

	//  spot lights
	for(int i = 0; i < nbSpotLights; i++)
	{
		result += ComputeSpotLight(spotLights[i], normal, viewDir, tFragPos, texCoord);
	}

	return result;
}


vec3 ComputeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec2 texCoord)
{
	//  ambient
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoord));

	//  diffuse
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoord));

	//  specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoord));

	//  result
	vec3 result = ambient + diffuse + specular;
	return result;
}

vec3 ComputePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos, vec2 texCoord)
{
	//  ambient
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoord));

	//  diffuse
	vec3 lightDir = normalize(light.position - tFragPos);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoord));

	//  specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoord));

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

vec3 ComputeSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos, vec2 texCoord)
{
	//  ambient
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoord));

	//  diffuse
	vec3 lightDir = normalize(light.position - tFragPos);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoord));

	//  specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoord));
    
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