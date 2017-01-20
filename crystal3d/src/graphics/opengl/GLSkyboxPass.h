#pragma once
#include "resources\Skybox.h"

#include "GLRenderPass.h"

namespace Graphics
{
	namespace OpenGL
	{
		struct GLSkyboxPassContext 
			: GLRenderPassContext
		{
			Resources::CrSkybox* skybox;
		};

		class GLSkyboxPass 
			: public GLRenderPass
		{
		public:
			GLSkyboxPass(GLSkyboxPassContext& a_Context);
			~GLSkyboxPass();
		};
	}
}