#include "GLPipelineScript.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLPipelineScript::GLPipelineScript()
			: CrScript::CrScript()
		{

		}

		GLPipelineScript::~GLPipelineScript()
		{
		}

		void GLPipelineScript::LoadFromFile(const std::string & a_File, Resources::ResourceCreateInfo * a_Info)
		{
			CrScript::LoadFromFile(a_File, a_Info);

			GLPipelineScriptCreateInfo* info = static_cast<GLPipelineScriptCreateInfo*>(a_Info);

			m_State.new_usertype<GLRenderPassContext>("RenderPassContext",
				"VertexShader", &GLRenderPassContext::vertexShaderFile,
				"FragmentShader", &GLRenderPassContext::fragmentShaderFile,
				"ViewportWidth", &GLRenderPassContext::viewportWidth,
				"ViewportHeight", &GLRenderPassContext::viewportHeight,
				"BufferFlags", &GLRenderPassContext::bufferFlags);

			m_State.new_usertype<GLRenderPass, GLRenderPassContext>("RenderPass");

			m_State.new_usertype<GLSkyboxPassContext>("SkyboxPassContext",
				"Skybox", &GLSkyboxPassContext::skybox, SOL_BASE(GLRenderPassContext));
			m_State.new_usertype<GLSkyboxPass, GLSkyboxPassContext>("SkyboxPass");
			m_State.new_usertype<GLSSAOPassContext>("SSAOPassContext", SOL_BASE(GLRenderPassContext));
			m_State.new_usertype<GLSSAOPass, GLSSAOPassContext>("SSAOPass");

			m_State["VIEWPORT_WIDTH"] = info->viewportWidth;
			m_State["VIEWPORT_HEIGHT"] = info->viewportHeight;
			m_State["BUFFER_POINT_LIGHT"] = BUFFER_FLAG_POINT_LIGHT;
			m_State["BUFFER_DIRECTIONAL_LIGHT"] = BUFFER_FLAG_DIRECTIONAL_LIGHT;
			m_State["BUFFER_LIGHT_INFO"] = BUFFER_FLAG_LIGHT_INFO;
			m_State["BUFFER_GEOMETRY"] = BUFFER_FLAG_GEOMETRY;
			m_State["BUFFER_PREVIOUS_PASS"] = BUFFER_FLAG_PREVIOUS_PASS;
			m_State["BUFFER_DEPTH"] = BUFFER_FLAG_DEPTH;
			m_State["BUFFER_FLAG_UTILITY"] = BUFFER_FLAG_UTILITY;

			m_State["CreateRenderPass"] = info->rpCreateFunc;
		}
	}
}
