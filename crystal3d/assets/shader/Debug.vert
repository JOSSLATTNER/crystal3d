#version 420

in vec3 position;
out vec3 fragPosition;

//Model, View, Projection
layout(std140) uniform MVPBuffer
{
	mat4 transformMatrix;
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

void main()
{
	fragPosition = position;
	gl_Position = vec4(position,1)*viewMatrix*projectionMatrix;
}