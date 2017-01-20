#include "GLDeferredRenderer.h"
#include "GLRenderer.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{

		GLDeferredRenderer::GLDeferredRenderer()
		{
			m_ViewportWidth = 0;
			m_ViewportHeight = 0;
			m_PointLightUniformBuffer = new GLUniformBuffer<Scene::CrPointLight>("PointLightBlock", GL_DYNAMIC_DRAW);
			m_DirectionalLightUniformBuffer = new GLUniformBuffer<Scene::CrDirectionalLight>("DirectionalLightBlock", GL_DYNAMIC_DRAW);
			m_LightInfoBuffer = new GLUniformBuffer<GLDeferredLightInfo>("LightInfoBlock", GL_DYNAMIC_DRAW);
			m_RenderPassFramebuffer = nullptr;
		}

		GLDeferredRenderer::~GLDeferredRenderer()
		{
			delete m_PointLightUniformBuffer;
			delete m_DirectionalLightUniformBuffer;
			delete m_LightInfoBuffer;
			delete m_RenderPassFramebuffer;
		}

		void GLDeferredRenderer::Initialize(GLDeferredRendererContext& a_Context)
		{
			m_ViewportWidth = a_Context.viewportWidth;
			m_ViewportHeight = a_Context.viewportHeight;
			m_GeometryBuffer = a_Context.geometryBuffer;

			GLFramebufferContext ctx{};
			ctx.formats = {GL_RGBA};
			ctx.height = m_ViewportHeight;
			ctx.width = m_ViewportWidth;
			ctx.useDepthTexture = false;

			m_RenderPassFramebuffer = new GLFramebuffer(ctx);
	
			GLPipelineScriptCreateInfo info{};
			info.viewportHeight = m_ViewportHeight;
			info.viewportWidth = m_ViewportWidth;
			info.rpCreateFunc = [&](GLRenderPass* pass)
			{
				this->CreateRenderPass(pass);
			};

			auto pResources = SEngine->GetResourceManager();
			m_PipelineScript = pResources->FetchResource<GLPipelineScript>("Scripts\\Pipeline.lua", &info);
			
			m_PipelineScript->Run();
			m_PipelineScript->Invoke<void>("Init");
		}

		void GLDeferredRenderer::CreateRenderPass(GLRenderPass* a_RenderPass)
		{
			unsigned char bufferFlags = a_RenderPass->m_Context.bufferFlags;
			auto spHandle = a_RenderPass->m_ShaderProgram->GetHandle();

			if (BIT_HAS_FLAG(bufferFlags, BUFFER_FLAG_DIRECTIONAL_LIGHT))
			{
				m_DirectionalLightUniformBuffer->Bind(spHandle);
			}

			if (BIT_HAS_FLAG(bufferFlags, BUFFER_FLAG_POINT_LIGHT))
			{
				m_PointLightUniformBuffer->Bind(spHandle);
			}

			if (BIT_HAS_FLAG(bufferFlags, BUFFER_FLAG_LIGHT_INFO))
			{
				m_LightInfoBuffer->Bind(spHandle);
			}

			if (BIT_HAS_FLAG(bufferFlags, BUFFER_FLAG_UTILITY))
			{
				GLRenderer::m_UniformUtilityBuffer->Bind(spHandle);
			}

			if (BIT_HAS_FLAG(bufferFlags, BUFFER_FLAG_GEOMETRY))
			{
				auto gBufferTextures = m_GeometryBuffer->GetTextures();
				for (uint32_t i = 0; i < gBufferTextures.size(); i++)
				{
					a_RenderPass->m_ShaderProgram->AttachTexture(gBufferTextures[i], i);
				}
			}

			auto depthTexture = m_GeometryBuffer->GetDepthTexture();
			if (BIT_HAS_FLAG(bufferFlags, BUFFER_FLAG_DEPTH)
				&& depthTexture != nullptr)
			{
				a_RenderPass->m_ShaderProgram->AttachTexture(depthTexture, "tDepthTexture");
			}

			if (BIT_HAS_FLAG(bufferFlags, BUFFER_FLAG_PREVIOUS_PASS))
			{
				GLTexture2D* previousPass = m_RenderPassFramebuffer->GetTextures()[0];
				a_RenderPass->m_ShaderProgram->AttachTexture(previousPass, "tPreviousPass");
			}

			m_RenderPasses.push_back(a_RenderPass);
		}

		void GLDeferredRenderer::Render(Scene::CrSceneRenderInfo& a_Info)
		{
			this->UpdateLightBuffer(a_Info);

			for (uint32_t i = 0; i < m_RenderPasses.size() - 1; i++)
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				m_RenderPassFramebuffer->Bind();
				m_RenderPasses[i]->Render();
				m_RenderPassFramebuffer->Unbind();
			}

			//Final pass: draw into backbuffer
			m_RenderPasses.back()->Render();
		}

		void GLDeferredRenderer::UpdateLightBuffer(Scene::CrSceneRenderInfo& a_Info) const
		{

			GLDeferredLightInfo lightInfo;
			lightInfo.hasDirectionalLight = uint32_t(*a_Info.m_DirectionalLight != nullptr);
			lightInfo.numPointLights = float(a_Info.m_PointLights->size());
			lightInfo.cameraPosition = (*a_Info.m_Camera)->m_Transform.Translation;

			m_LightInfoBuffer->Subdata(&lightInfo, 0);
		
			//TODO: UNIFORM ENTIRE ARRAY
			if (lightInfo.numPointLights > 0)
			{
				m_PointLightUniformBuffer->Subdata(a_Info.m_PointLights->at(0), 0);
			}
			if (lightInfo.hasDirectionalLight)
			{
				m_DirectionalLightUniformBuffer->Subdata(*a_Info.m_DirectionalLight, 0);
			}
		}
	}
}
