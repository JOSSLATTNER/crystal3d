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
			m_PointLightBuffer = new GLUniformBuffer<PointLight>("PointLightBlock", GL_DYNAMIC_DRAW);
			m_DirectionalLightBuffer = new GLUniformBuffer<DirectionalLight>("DirectionalLightBlock", GL_DYNAMIC_DRAW);
			m_LightInfoBuffer = new GLUniformBuffer<LightInfo>("LightInfoBlock", GL_DYNAMIC_DRAW);
			m_RenderPassFramebuffer = nullptr;
		}

		GLDeferredRenderer::~GLDeferredRenderer()
		{
			delete m_PointLightBuffer;
			delete m_DirectionalLightBuffer;
			delete m_LightInfoBuffer;
			delete m_RenderPassFramebuffer;

			for (auto& rp : m_RenderPasses)
			{
				delete rp;
			}
		}

		void GLDeferredRenderer::Initialize(GLDeferredRendererContext& a_Context)
		{
			m_ViewportWidth = a_Context.viewportWidth;
			m_ViewportHeight = a_Context.viewportHeight;
			m_GeometryBuffer = a_Context.geometryBuffer;

			GLFramebufferContext ctx{};
			ctx.formats = { GL_RGBA };
			ctx.height = m_ViewportHeight;
			ctx.width = m_ViewportWidth;
			ctx.useDepthTexture = false;

			m_RenderPassFramebuffer = new GLFramebuffer(ctx);

			//Lighting pass
			GLRenderPassContext lpCx{};
			lpCx.vertexShaderFile = "shader\\Deferred.vert";
			lpCx.fragmentShaderFile = "shader\\Lighting.frag";
			lpCx.viewportHeight = m_ViewportHeight;
			lpCx.viewportWidth = m_ViewportWidth;

			GLRenderPass* lightingPass = new GLRenderPass(lpCx);
			this->RegisterRenderPass(lightingPass, BUFFER_FLAG_POINT_LIGHT | BUFFER_FLAG_DIRECTIONAL_LIGHT | BUFFER_FLAG_GEOMETRY | BUFFER_FLAG_LIGHT_INFO);

			//Skybox pass
			GLRenderPassContext sbpCx{};
			sbpCx.vertexShaderFile = "shader\\Deferred.vert";
			sbpCx.fragmentShaderFile = "shader\\Skybox.frag";
			sbpCx.viewportHeight = m_ViewportHeight;
			sbpCx.viewportWidth = m_ViewportWidth;

			GLRenderPass* skyboxPass = new GLRenderPass(sbpCx);
			this->RegisterRenderPass(skyboxPass, BUFFER_FLAG_PREVIOUS_PASS | BUFFER_FLAG_GEOMETRY | BUFFER_FLAG_DEPTH);
		}

		void GLDeferredRenderer::RegisterRenderPass(GLRenderPass* a_RenderPass, unsigned int a_BufferFlags)
		{
			auto spHandle = a_RenderPass->m_ShaderProgram->GetHandle();

			if (BIT_HAS_FLAG(a_BufferFlags, BUFFER_FLAG_DIRECTIONAL_LIGHT))
			{
				m_DirectionalLightBuffer->Bind(spHandle);
			}

			if (BIT_HAS_FLAG(a_BufferFlags, BUFFER_FLAG_POINT_LIGHT))
			{
				m_PointLightBuffer->Bind(spHandle);
			}

			if (BIT_HAS_FLAG(a_BufferFlags, BUFFER_FLAG_LIGHT_INFO))
			{
				m_LightInfoBuffer->Bind(spHandle);
			}

			if (BIT_HAS_FLAG(a_BufferFlags, BUFFER_FLAG_UTILITY))
			{
				GLRenderer* myRenderer = static_cast<GLRenderer*>(SEngine->GetRenderer());
				myRenderer->GetUtilBuffer()->Bind(spHandle);
			}

			if (BIT_HAS_FLAG(a_BufferFlags, BUFFER_FLAG_GEOMETRY))
			{
				auto gBufferTextures = m_GeometryBuffer->GetTextures();
				for (uint32_t i = 0; i < gBufferTextures.size(); i++)
				{
					a_RenderPass->m_ShaderProgram->AttachTexture(gBufferTextures[i], i);
				}
			}

			auto depthTexture = m_GeometryBuffer->GetDepthTexture();
			if (BIT_HAS_FLAG(a_BufferFlags, BUFFER_FLAG_DEPTH)
				&& depthTexture != nullptr)
			{
				a_RenderPass->m_ShaderProgram->AttachTexture(depthTexture, "tDepthTexture");
			}

			if (BIT_HAS_FLAG(a_BufferFlags, BUFFER_FLAG_PREVIOUS_PASS))
			{
				GLTexture2D* previousPass = m_RenderPassFramebuffer->GetTextures()[0];
				a_RenderPass->m_ShaderProgram->AttachTexture(previousPass, "tPreviousPass");
			}

			m_RenderPasses.push_back(a_RenderPass);
		}

		void GLDeferredRenderer::Render(Scene::CrScene* a_Scene)
		{
			this->UpdateLightBuffer(a_Scene);

			for (uint32_t i = 0; i < m_RenderPasses.size() - 1; i++)
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				m_RenderPassFramebuffer->Bind();
				m_RenderPasses[i]->Render();
				m_RenderPassFramebuffer->Unbind();
			}

			m_RenderPasses.back()->Render();
		}

		void GLDeferredRenderer::UpdateLightBuffer(Scene::CrScene* a_Scene) const
		{
			auto camNode = a_Scene->GetNode<Scene::CrCameraNode>();

			LightInfo lightInfo;
			lightInfo.hasDirectionalLight = uint32_t(true);
			lightInfo.numPointLights = uint32_t(1);
			lightInfo.cameraPosition = camNode->m_Transform.Translation;

			m_LightInfoBuffer->Subdata(&lightInfo, 0);

			PointLight pointLight;
			pointLight.diffuse = { 1.0f, 0.0f, 0.0f };
			pointLight.position = { 0, 0, 0 };
			pointLight.radius = 10;
			pointLight.specular = { 1.0f, 1.0f, 1.0f };

			m_PointLightBuffer->Subdata(&pointLight);
		
			DirectionalLight dirLight;
			dirLight.diffuse = { 1.0f, 1.0f, 1.0f };
			dirLight.direction = { 1.0f, 0.0f, 1.0f };
			dirLight.ambientFactor = 0.6f;
			dirLight.specularFactor = 0.0f;

			m_DirectionalLightBuffer->Subdata(&dirLight);

		}
	}
}
