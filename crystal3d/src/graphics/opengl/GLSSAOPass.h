#pragma once
#include "math/glm/GLM.h"

#include "GLRenderPass.h"
#include "GLUniformBuffer.hpp"

#define KERNEL_SIZE 64

namespace Graphics
{
	namespace OpenGL
	{
		struct GLSSAOPassContext 
			: GLRenderPassContext
		{
			//TODO: Settings
		};

		class GLSSAOPass 
			: public GLRenderPass
		{
		public:
			GLSSAOPass(GLSSAOPassContext& a_Context);
			~GLSSAOPass();

		//private:
		//	GLTexture2D* m_NoiseTexture;

		//	struct alignas(16) SSAOKernel
		//	{
		//		alignas(16) glm::vec4 values[KERNEL_SIZE];
		//	};

		//	GLUniformBuffer<SSAOKernel>* m_KernelBuffer;

		};
	}
}