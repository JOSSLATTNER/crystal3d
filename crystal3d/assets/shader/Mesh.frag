#version 420

//In
in vec2 fragUV;
in vec3 fragNormal;
in vec3 fragVertex;

//Out (->Geometry Buffer)
out vec4 [4] output;

//Textures
uniform sampler2D tDiffuse;
uniform sampler2D tSpecular;
uniform sampler2D tNormal;

//MVP
layout(std140) uniform MVPBuffer {
	mat4 transformMatrix;
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

// http://www.thetenthplanet.de/archives/1180
mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
 
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    // construct a scale-invariant frame 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}

vec3 perturb_normal( vec3 N, vec3 V, vec2 texcoord )
{
    // assume N, the interpolated vertex normal and 
    // V, the view vector (vertex to eye)
   vec3 map = texture(tNormal, texcoord ).xyz;
   map = map * 255./127. - 128./127.;
    mat3 TBN = cotangent_frame(N, -V, texcoord);
    return normalize(TBN * map);
}

vec3 DoNormalMapping(vec3 normal, vec3 sampledNormal)
{
	//FAKE TANGENT (0,1,0)
	vec3 tangent = cross(sampledNormal, vec3(0,1,0));
	vec3 bitangent = cross(sampledNormal, tangent);
	mat3 TBN = mat3(tangent, bitangent, sampledNormal); 

	return normalize(normal*TBN);
}

vec3 GetNormal()
{
	mat3 rotation = mat3(transformMatrix);
	
	//TODO: USE PRECOMPUTED TANGENTS

	//#1: http://www.thetenthplanet.de/archives/1180
	vec3 camPos = inverse(viewMatrix)[3].xyz;
	vec3 view = normalize(camPos - fragVertex.xyz);

	vec3 mappedNormal = perturb_normal(fragNormal, view, fragUV);

	//#2: FAKE TANGENT MAPPING
	//vec3 sampledNormal = texture2D(tNormal, fragUV).xyz;
	//vec3 mappedNormal = DoNormalMapping(fragNormal, sampledNormal);

	vec3 rotatedNormal = normalize(mappedNormal * rotation);
	return rotatedNormal;
}

void main ()
{
	output[0] = vec4(texture2D(tDiffuse,fragUV).rgb, 0);
	output[1] = vec4(texture2D(tSpecular, fragUV).rgb, 0);
	output[2] = vec4(fragVertex,0);
	output[3] = vec4(GetNormal(),0);
}