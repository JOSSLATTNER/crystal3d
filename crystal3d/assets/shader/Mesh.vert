#version 420
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

//IN
in vec3 inVertex;
in vec2 inUV;
in vec3 inNormal;

//Model, View, Projection
layout(std140) uniform MVPBuffer
{
	mat4 transformMatrix;
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

//OUT
out vec2 fragUV;
out vec3 fragNormal;
out vec3 fragVertex;

void main ()
{	
	vec4 world = vec4(inVertex,1) * transformMatrix;

	fragUV = inUV;
	fragNormal = inNormal;
	fragVertex = world.xyz;

	gl_Position = world * viewMatrix * projectionMatrix;
};