#include "GLSkyboxPass.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLSkyboxPass::GLSkyboxPass(GLSkyboxPassContext& a_Context)
			: GLRenderPass::GLRenderPass(a_Context)
		{
			//auto pResources = SEngine->GetResourceManager();
			//auto cmp = pResources->FetchResource<GLCubemap>(a_Context.skybox->cubemap, nullptr);
			//m_ShaderProgram->AttachCubemap(cmp, "skybox");
		}

		GLSkyboxPass::~GLSkyboxPass()
		{
		}
	}
}