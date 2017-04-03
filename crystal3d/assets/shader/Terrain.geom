#version 330

#define rot(x) mat2(cos(x), -sin(x), sin(x), cos(x))

//LAYOUT
layout (triangles) in;
layout (triangle_strip, max_vertices = 11) out;


//UNIFORM
uniform float gTime;
uniform sampler2D tGrassMap;

//IN
in VertexData{
	vec2  UV;
	vec3  Normal;
	vec3  Position;
	float TextureBlend;
} VertexIn[];

//OUT
out VertexData{
	vec2  UV;
	vec3  Normal;
	vec3  Position;
	float TextureBlend;
} VertexOut;

//Model, View, Projection
layout(std140) uniform MVPBuffer
{
	mat4 transformMatrix;
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

//VP
vec4 VP(vec4 inVertex)
{
	return inVertex*viewMatrix*projectionMatrix;
}

void main()
{
	for (int i = 0; i < 3; i++)
	{
		gl_Position = VP(gl_in[i].gl_Position);
		VertexOut.Position = VertexIn[i].Position;
		VertexOut.Normal = VertexIn[i].Normal;
		VertexOut.UV = VertexIn[i].UV;
		VertexOut.TextureBlend = VertexIn[i].TextureBlend;

		EmitVertex();
	}
	EndPrimitive();


	//Billboards
	vec3 center = (gl_in[0].gl_Position.xyz+
				   gl_in[1].gl_Position.xyz+
				   gl_in[2].gl_Position.xyz)/3.;

	
	vec4 noise = texture2D(tGrassMap,VertexIn[0].UV);

	if (mod(ceil(center).xz,7.)==0 && noise.r > 0.6)
	{

		vec2 billboardSize = vec2(20,22.);
		vec3 billboardNormal = normalize(vec3(0,0,1));
		vec3 billboardOffset = vec3(0,billboardSize.y/2.+1.,0);
		
		center += billboardOffset;
		float billboardSkew = 0.; //sin(gTime+center.x+center.z+noise.r)*10.;

		mat4 billboardMatrix;
		billboardMatrix[0] = vec4(viewMatrix[0][0], viewMatrix[0][1], viewMatrix[0][2], 0);
		billboardMatrix[1] = vec4(viewMatrix[1][0], viewMatrix[1][1], viewMatrix[1][2], 0);
		billboardMatrix[2] = vec4(viewMatrix[2][0], viewMatrix[2][1], viewMatrix[2][2], 0);
		billboardMatrix[3] = vec4(center, 1);

		billboardMatrix = transpose(billboardMatrix);


		//A
		VertexOut.Position = vec3(-billboardSize.x,-billboardSize.y,0);
		VertexOut.Position.z += billboardSkew;
		VertexOut.Normal = billboardNormal;
		VertexOut.UV = vec2(0,0);
		VertexOut.TextureBlend = 2.;

		gl_Position = VP(vec4(VertexOut.Position,1)*billboardMatrix);
		EmitVertex();

		//C
		VertexOut.Position = vec3(-billboardSize.x,billboardSize.y,0);
		VertexOut.Position.z += billboardSkew;
		VertexOut.Normal = billboardNormal;
		VertexOut.UV = vec2(0,1);
		VertexOut.TextureBlend = 2.;

		gl_Position = VP(vec4(VertexOut.Position,1)*billboardMatrix);
		EmitVertex();

		//B
		VertexOut.Position = vec3(billboardSize.x,-billboardSize.y,0);
		VertexOut.Normal = billboardNormal;
		VertexOut.UV = vec2(1,0);
		VertexOut.TextureBlend = 2.;

		gl_Position = VP(vec4(VertexOut.Position,1)*billboardMatrix);
		EmitVertex();

		//D
		VertexOut.Position = vec3(billboardSize.x,billboardSize.y,0);
		VertexOut.Normal = billboardNormal;
		VertexOut.UV = vec2(1,1);
		VertexOut.TextureBlend = 2.;

		gl_Position = VP(vec4(VertexOut.Position,1)*billboardMatrix);
		EmitVertex();

		////A2
		//VertexOut.Position = center+vec3(0,-billboardSize.y,-billboardSize.x);
		//VertexOut.Position.z += billboardSkew;
		//VertexOut.Normal = billboardNormal;
		//VertexOut.UV = vec2(0,0);
		//VertexOut.TextureBlend = 2.;
		//
		//gl_Position = VP(vec4(VertexOut.Position,1));
		//EmitVertex();
		//
		////C2
		//VertexOut.Position = center+vec3(0,billboardSize.y,-billboardSize.x);
		//VertexOut.Position.z += billboardSkew;
		//VertexOut.Normal = billboardNormal;
		//VertexOut.UV = vec2(0,1);
		//VertexOut.TextureBlend = 2.;
		//
		//gl_Position = VP(vec4(VertexOut.Position,1));
		//EmitVertex();
		//
		////B2
		//VertexOut.Position = center+vec3(0,-billboardSize.y,billboardSize.x);
		//VertexOut.Normal = billboardNormal;
		//VertexOut.UV = vec2(1,0);
		//VertexOut.TextureBlend = 2.;
		//
		//gl_Position = VP(vec4(VertexOut.Position,1));
		//EmitVertex();
		//
		////D2
		//VertexOut.Position = center+vec3(0,billboardSize.y,billboardSize.x);
		//VertexOut.Normal = billboardNormal;
		//VertexOut.UV = vec2(1,1);
		//VertexOut.TextureBlend = 2.;
		//
		//gl_Position = VP(vec4(VertexOut.Position,1));
		//EmitVertex();

		EndPrimitive();

	}

}