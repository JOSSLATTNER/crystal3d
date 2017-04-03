#version 420
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

//0 = ALL
//1 = DIFFUSE
//2 = NORMALS
//3 = POSITION
//4 = DEPTH
//5 = RED
//6 = SPECULAR
#define MODE 1

//IN
in vec2 fragCoord;

//OUT
layout(location = 0) out vec4 fragColor;

//G-Buffer Textures
layout(location = 0) uniform sampler2D tDiffuse; 
layout(location = 1) uniform sampler2D tSpecular;
layout(location = 2) uniform sampler2D tPosition;
layout(location = 3) uniform sampler2D tNormals;

//Previous pass
uniform sampler2D tPreviousPass;

//Depth texture
uniform sampler2D tDepthTexture;

//POINT LIGHTS
layout(std140) uniform PointLightBlock
{    
	vec3 position;
	float radius;
	vec3 diffuse;
	vec3 specular;
} pointLight;

//DIRECTIONAL LIGHTS
layout(std140) uniform DirectionalLightBlock
{    
	vec3 direction;
	float ambientFactor;
    vec3 diffuse;
	float specular;
} directionalLight;

//LIGHT INFO
layout(std140) uniform LightInfoBlock
{
	bool hasDirectionalLight;
	vec3 cameraPosition;
	uint numPointLights;
} lightInfo;

vec3 CalcPointLight(int i, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(pointLight.position-fragPos);
	
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
	
    // Specular shading
    vec3 reflectDir = normalize(reflect(lightDir, normal));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2.0);
	
    // Attenuation
    float distance = length(pointLight.position - fragPos);
	
	float radius = pointLight.radius;
	float attenuation = clamp(1.0 - (distance*distance)/(radius*radius), 0.0, 1.0);
	attenuation *= attenuation;

    // Combine results
    vec3 diffuse  = pointLight.diffuse  * diff * vec3(texture2D(tDiffuse, fragCoord));
    vec3 specular = pointLight.specular * spec * vec3(texture2D(tSpecular, fragCoord));
	
    vec3 result = attenuation*(diffuse+specular);
	return result;
} 

vec3 CalcDirLight(vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-directionalLight.direction);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = normalize(reflect(lightDir, normal));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2.0);

    // Combine results
	float gamma = 2.2;
	vec3 diffuseColor = pow(texture2D(tDiffuse,  fragCoord).rgb, vec3(gamma));

    vec3 ambient  = directionalLight.ambientFactor  * texture2D(tDiffuse, fragCoord).rgb;
    vec3 diffuse  = directionalLight.diffuse * diff * diffuseColor;
    vec3 specular = directionalLight.specular * spec * texture2D(tSpecular, fragCoord).rgb;

    vec3 result = ambient+(diffuse+specular);
	return result;
}

void main()
{
	vec4 position = texture2D( tPosition, fragCoord );
	vec4 normal = normalize(texture2D( tNormals,fragCoord));
	vec3 camDir = normalize(lightInfo.cameraPosition-position.xyz);
	
	vec3 result = vec3(0); //texture2D(tPreviousPass, fragCoord).rgb; (cool effect)

	if (true /*lightInfo.hasDirectionalLight==1*/)
		result = CalcDirLight(normal.xyz, camDir);

	for(int i = 0; i < lightInfo.numPointLights; i++)
		result += CalcPointLight(i, normal.xyz, position.xyz, camDir);
	
	#if MODE==1
		result = texture2D(tDiffuse, fragCoord).rgb;
	#elif MODE==2
		result = normal.xyz;
	#elif MODE==3
		result = position.xyz;
	#elif MODE==4
		result = texture2D(tDepthTexture, fragCoord).rgb;
	#elif MODE==5
		result = vec3(1,0,0);
	#elif MODE==6
		result = texture2D(tSpecular, fragCoord).rgb;
	#endif

	fragColor = vec4(result,1);
}