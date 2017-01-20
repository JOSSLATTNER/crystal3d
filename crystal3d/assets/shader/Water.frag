#version 420

//In
in vec2 fragUV;
in vec3 fragNormal;
in vec3 fragVertex;

//Out (->Geometry Buffer)
out vec4 [4] output;

//Textures
uniform sampler2D tDiffuse;

void main ()
{
	output[0] = vec4(texture2D(tDiffuse,fragUV).rgb,0);
	output[1] = vec4(1,1,1,0);
	output[2] = vec4(fragVertex,0);
	output[3] = vec4(fragNormal,0);
}