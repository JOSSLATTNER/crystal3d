#version 330 compatibility

//IN
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;

//UNIFORM
uniform sampler2D tHeightmap;
uniform sampler2D tNormalMap;
uniform float displacementScale;

//Model, View, Projection
layout(std140) uniform MVPBuffer
{
	mat4 transformMatrix;
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

//OUT (->Fragment Shader)
out VertexData{
	vec2  UV;
	vec3  Normal;
	vec3  Position;
	float TextureBlend;
} VertexOut;

void main()
{
	float sampledHeight = texture2D(tHeightmap, inUV).r;

	vec3 displacement = vec3(0,sampledHeight*displacementScale,0);
	vec3 pos = inPosition+displacement;
	vec4 world = vec4(pos,1)*transformMatrix;
	vec3 sampledNormal = texture2D(tNormalMap, inUV).xyz;

	VertexOut.UV = inUV;
	VertexOut.Normal = sampledNormal;
	VertexOut.TextureBlend = sampledHeight;
	VertexOut.Position = world.xyz;

	gl_Position = world;
}