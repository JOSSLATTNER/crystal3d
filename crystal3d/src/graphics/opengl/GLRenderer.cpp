#include "GLRenderer.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{
		//TODO: REMOVE STATIC..
		GLUniformBuffer<GLRenderer::ShaderUtil>* GLRenderer::m_UniformUtilityBuffer = nullptr;
		GLUniformBuffer<GLRenderer::MVP>* GLRenderer::m_UniformMVPBuffer = nullptr;

		GLRenderer::GLRenderer()
		{
			m_CurrentContext = nullptr;
			m_UniformMVPBuffer = nullptr;
			m_UniformUtilityBuffer = nullptr;
			m_GeometryBuffer = nullptr;
			m_DeferredRenderer = nullptr;
		}

		GLRenderer::~GLRenderer()
		{
			delete m_UniformMVPBuffer;
			delete m_UniformUtilityBuffer;
			delete m_GeometryBuffer;
			delete m_DeferredRenderer;
			delete m_CurrentContext;

			for (auto& re : m_RenderEntities)
			{
				delete re;
			}
		}

		bool GLRenderer::Initialize(CrRendererContext& a_Context)
		{
			m_CurrentContext = new GLContext();
			if (!m_CurrentContext->CreateContext(a_Context.windowHandle))
			{
				CrAssert(0, "Could not create OpenGL Context!");
				return false;
			}

			GLenum err = glewInit();

			if (GLEW_OK != err)
			{
				CrAssert(0, "Glew failed to initialize: %s", glewGetErrorString(err));
				return false;
			}

			CrLog("Version: %s", glGetString(GL_VERSION));

			//@GL Settings
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_TEXTURE_2D);
			glDepthFunc(GL_LESS); 
			glClearColor(0., 0., 0., 1.);
			glViewport(0, 0, a_Context.viewportWidth, a_Context.viewportHeight);

#ifdef GL_DEBUG
			glEnable(GL_DEBUG_OUTPUT);

			glDebugMessageCallback(ErrorCallback, NULL);

			GLuint unusedIds = 0;
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
				GL_DONT_CARE, 0, &unusedIds, GL_TRUE);
#endif

			GLFramebufferContext ctx{};
			ctx.formats = { G_BUFFER_FORMATS };
			ctx.width = a_Context.viewportWidth;
			ctx.height = a_Context.viewportHeight;
			ctx.useDepthTexture = true;

			m_GeometryBuffer = new GLFramebuffer(ctx);
			m_UniformMVPBuffer = new GLUniformBuffer<MVP>("MVPBuffer", GL_DYNAMIC_DRAW);
			m_UniformUtilityBuffer = new GLUniformBuffer<ShaderUtil>("UtilBuffer", GL_DYNAMIC_DRAW);

			GLDeferredRendererContext deferredContext{};
			deferredContext.viewportHeight = a_Context.viewportHeight;
			deferredContext.viewportWidth = a_Context.viewportWidth;
			deferredContext.geometryBuffer = m_GeometryBuffer;

			m_DeferredRenderer = new GLDeferredRenderer();
			m_DeferredRenderer->Initialize(deferredContext);

			return true;
		}


		void GLRenderer::Render(Scene::CrSceneRenderInfo& a_Info)
		{
			/*
			Deferred Pipeline:
			1) Geometry Pass: Renders geometry into framebuffer object (FBO)
			2) Lighting Pass: Applies Lighting to geometry
			3) Additional Passes: Shadows, Reflections, PostFX
			*/

			if ((*a_Info.m_Camera) == nullptr)
			{
				return;
			}

			m_UniformMVPBuffer->Subdata<glm::mat4>(&(*a_Info.m_Camera)->m_ViewMatrix, MVP_BUFFER_OFFSET_VIEW);
			m_UniformMVPBuffer->Subdata<glm::mat4>(&(*a_Info.m_Camera)->m_ProjectionMatrix, MVP_BUFFER_OFFSET_PROJECTION);

			auto pTimer = SEngine->GetGameTimer();
			float time = pTimer->GetTotal();
			m_UniformUtilityBuffer->Subdata<float>(&time,0);

			m_GeometryBuffer->Bind();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				for (auto& n : m_RenderEntities)
				{
					n->Render();
				}
			m_GeometryBuffer->Unbind();

			m_DeferredRenderer->Render(a_Info);
			m_CurrentContext->SwapBuffer();
		}

		void GLRenderer::AddRenderable(Graphics::IRenderable* a_Node)
		{
			auto mesh = a_Node->GetMesh();
			auto material = a_Node->GetMaterial();
			auto mode = a_Node->GetRenderMode();
			auto boundingBox = a_Node->GetBoundingBox();
			auto transform = a_Node->GetTransform();

			auto vao = new GLVertexArray(mesh);
			auto program = new GLShaderProgram(material);
	
			GLenum glMode;
			switch (mode)
			{
			case Graphics::ERenderMode::Triangles:
				glMode = GL_TRIANGLES;
				break;
			case Graphics::ERenderMode::Triangle_Strip:
				glMode = GL_TRIANGLE_STRIP;
				break;
			case Graphics::ERenderMode::Quads:
				glMode = GL_QUADS;
				break;
			case Graphics::ERenderMode::Triangle_Fan:
				glMode = GL_TRIANGLE_FAN;
				break;
			case Graphics::ERenderMode::Lines:
				glMode = GL_LINES;
				break;
			default:
				CrAssert(0, "Invalid Render Mode.");
				return;
			}

			auto entity = new GLRenderEntity(vao, program, glMode);
			entity->SetTransform(transform);
			entity->SetBoundingBox(boundingBox);

			m_RenderEntities.push_back(entity);
		}


	}
}