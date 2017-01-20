#include "GLSSAOPass.h"
#include "GLRenderer.h"
#include "core/Engine.h"

namespace Graphics
{
	namespace OpenGL
	{

		GLSSAOPass::GLSSAOPass(GLSSAOPassContext& a_Context)
			:GLRenderPass::GLRenderPass(a_Context)
		{
			//m_KernelBuffer = new GLUniformBuffer<SSAOKernel>("KernelBuffer", GL_DYNAMIC_DRAW);

			//std::uniform_real_distribution<GLfloat> random_Floats(0.0, 1.0); // generates random floats between 0.0 and 1.0
			//std::default_random_engine generator;
			//
			//SSAOKernel kernel;
			//for (GLuint i = 0; i < KERNEL_SIZE; ++i)
			//{
			//	glm::vec3 sample(random_Floats(generator) * 2.0 - 1.0, random_Floats(generator) * 2.0 - 1.0, random_Floats(generator));
			//	sample = glm::normalize(sample);
			//	sample *= random_Floats(generator);
			//	GLfloat scale = GLfloat(i) / 64.0f;

			//	scale = glm::lerp(0.1f, 1.0f, scale * scale);
			//	sample *= scale;
			//	kernel.values[i] = glm::vec4(sample,0);
			//}


			//m_KernelBuffer->Bind(m_ShaderProgram->GetHandle());
			//m_KernelBuffer->Subdata(&kernel, 0);

			//GLRenderer::m_UniformMVPBuffer->Bind(m_ShaderProgram->GetHandle());

			//std::vector<glm::vec3> ssaoNoise;
			//for (GLuint i = 0; i < 16; i++)
			//{
			//	glm::vec3 noise(
			//		random_Floats(generator) * 2.0 - 1.0,
			//		random_Floats(generator) * 2.0 - 1.0,
			//		0.0f);
			//	ssaoNoise.push_back(noise);
			//}	

			//m_NoiseTexture = new GLTexture2D(&ssaoNoise[0], 4, 4, GL_RGB32F, GL_RGB);

			//m_NoiseTexture->SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			//m_NoiseTexture->SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//m_NoiseTexture->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
			//m_NoiseTexture->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

			//m_ShaderProgram->AttachTexture(m_NoiseTexture, "tNoise");


		}

		GLSSAOPass::~GLSSAOPass()
		{
			//delete m_NoiseTexture;
			//delete m_KernelBuffer;
		}

	}
}