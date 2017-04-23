#version 420
#define NORMAL_MAPPING_METHOD 2

//In
in vec2 fragUV;
in vec3 fragNormal;
in vec3 fragVertex;

//Out (->Geometry Buffer)
out vec4 [4] gBuffer;

//Textures
uniform sampler2D u_diffuse;
uniform sampler2D u_specular;
uniform sampler2D u_normal;

//Model, View, Projection
layout(std140) uniform MVPBuffer
{
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

vec3 perturb_normal( sampler2D sampler, vec3 N, vec3 V, vec2 texcoord )
{
    // assume N, the interpolated vertex normal and 
    // V, the view vector (vertex to eye)
	vec3 map = texture(sampler, texcoord).xyz;
	map = map * 255./127. - 128./127.;
    mat3 TBN = cotangent_frame(N, -V, texcoord);
    return normalize(TBN * map);
}

vec3 do_normal_mapping(vec3 normal, vec3 tangent, vec3 sampledNormal)
{
	vec3 bitangent = cross(sampledNormal, tangent);
	mat3 TBN = mat3(tangent, bitangent, sampledNormal); 

	return normalize(normal * TBN);
}

vec3 GetNormal()
{
#if NORMAL_MAPPING_METHOD==1
	//#1: TANGENT NORMAL MAPPING
	vec3 sampledNormal = texture2D(u_normal, fragUV).xyz;
	vec3 tangent = vec3(1,0,0); //TODO: PRECALC TANGENT

	vec3 mappedNormal = do_normal_mapping(fragNormal, tangent, sampledNormal);
#else
	//#2: WITHOUT TANGENTS: http://www.thetenthplanet.de/archives/1180
	vec3 camPos = inverse(viewMatrix)[3].xyz;
	vec3 view = normalize(camPos - fragVertex.xyz);
	vec3 mappedNormal = perturb_normal(fragNormal, view, fragUV);
#endif

	mat3 rotation = mat3(transformMatrix);
	vec3 rotatedNormal = normalize(mappedNormal * rotation);

	return rotatedNormal;
}

void main ()
{
	gBuffer[0] = vec4(texture2D(u_diffuse, fragUV).rgb, 0);
	gBuffer[1] = vec4(texture2D(u_specular, fragUV).rgb, 0);
	gBuffer[2] = vec4(fragVertex,0);
	gBuffer[3] = vec4(GetNormal(),0);
}