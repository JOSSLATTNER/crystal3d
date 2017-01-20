#version 420

in vec3 fragPosition;
out vec4 output[4];

void main()
{
	output[0] = vec4(1,0,0,0);
	output[1] = vec4(0);
	output[2] = vec4(fragPosition, 0);
	output[3] = vec4(1,1,0,0);
}