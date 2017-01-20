#version 420
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

//IN
in vec2 fragCoord;

//OUT
layout(location = 0) out vec4 fragColor;

layout(location = 0) uniform sampler2D tDiffuse; 
layout(location = 1) uniform sampler2D tSpecular;
layout(location = 2) uniform sampler2D tPosition;
layout(location = 3) uniform sampler2D tNormals;

uniform sampler2D tPreviousPass;
uniform sampler2D tDepthTexture;
uniform sampler2D tNoise;

layout(std140) uniform KernelBuffer 
{
	vec4 samples[64];
};

layout(std140) uniform MVPBuffer
{
	mat4 transformMatrix;
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

//TODO: MAKE VARIABLE
#define kernelSize 64.0f
#define iterations 5

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float doAmbientOcclusion(vec2 tcoord, vec2 uv, vec3 p, vec3 cnorm)
{
    float scale = 0.5f;
	float bias = 0.1f;
	float intensity = 3.0f;

	vec3 diff = texture2D(tPosition, tcoord + uv).xyz - p;

    vec3 v = normalize(diff);
    float d = length(diff) * scale;

    return max(0.0,dot(cnorm,v)-bias) * (1.0/(1.0+d)) * intensity;
}

float ambientOcclusion()
{
    vec2 texCoord = fragCoord;
    vec3 p = texture2D(tPosition, texCoord.xy).xyz;
    vec3 n = texture2D(tNormals, texCoord.xy).xyz;
    vec2 rnd = normalize(vec2(rand(p.xy), rand(n.xy)));

    float ao = 0.0f;
    float rad = 1.0/p.z;
    vec2 vec[4]; 
    vec[0] = vec2(1.0,0.0); 
    vec[1] = vec2(-1.0,0.0); 
    vec[2] = vec2(0.0,1.0); 
    vec[3] = vec2(0.0,-1.0);

    for (int j = 0; j < iterations; ++j)
    {
		vec2 coord1 = reflect(vec[j],rnd)*rad;
		vec2 coord2 = vec2(coord1.x * 0.707 - coord1.y * 0.707, coord1.x * 0.707 + coord1.y*0.707);
      
		ao += doAmbientOcclusion(texCoord.xy,coord1*0.25, p, n);
		ao += doAmbientOcclusion(texCoord.xy,coord2*0.5, p, n);
		ao += doAmbientOcclusion(texCoord.xy,coord1*0.75, p, n);
		ao += doAmbientOcclusion(texCoord.xy,coord2, p, n);
    }
		
	ao /= float(iterations)*4.0;
    return 1.0f - ao;
}

void main()
{
	vec3 prev = texture2D(tPreviousPass, fragCoord).rgb;
	//float occ = ambientOcclusion();
	//fragColor = vec4(ambientOcclusion());
	fragColor = vec4(prev, 1);
}