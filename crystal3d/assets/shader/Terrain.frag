#version 330 compatibility

//UNIFORM
uniform sampler2D tDiffuseTextures[2];
uniform sampler2D tNormalMaps[2];
uniform sampler2D tGrassTexture;

//IN
in VertexData{
	vec2  UV;
	vec3  Normal;
	vec3  Position;
	float TextureBlend;
} VertexIn;

//OUT (->G-Buffer)
out vec4 [4] output;

#define TextureScale 20
#define TextureBlendFactor VertexIn.TextureBlend-0.3
#define TextureNormalBlendFactor 0.5

vec4 BlendTextures()
{	
	//Grass
	if (VertexIn.TextureBlend > 0.999999)
	{
		vec4 grassTexture = texture2D(tGrassTexture,VertexIn.UV);
		if (grassTexture.a < 0.5)
		{
			discard;
		}
		return grassTexture;
	}

	//Terrain
	vec4 tex01 = texture2D(tDiffuseTextures[0],VertexIn.UV*TextureScale);
	vec4 tex02 = texture2D(tDiffuseTextures[1],VertexIn.UV*TextureScale);

	return mix(tex01,tex02, TextureBlendFactor);
}

vec3 BlendNormals()
{
	
	if (VertexIn.TextureBlend > 0.999999)
	{
		return VertexIn.Normal; //Use billboard normal
	}

	vec3 n01 = normalize(texture2D(tNormalMaps[0], VertexIn.UV*TextureScale)).xyz;
	vec3 n02 = normalize(texture2D(tNormalMaps[1], VertexIn.UV*TextureScale)).xyz;

	vec3 nmix = normalize(mix(n01, n02, TextureBlendFactor)).xyz;

	return mix(VertexIn.Normal, nmix, TextureNormalBlendFactor);
}

void main()
{
	output[0] = BlendTextures();
	output[1] = vec4(1,1,1, 0);
	output[2] = vec4(VertexIn.Position,0);
	output[3] = vec4(BlendNormals(),0);
}