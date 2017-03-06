#include "GLRenderer.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLRenderer::GLRenderer()
		{
			m_CurrentContext = nullptr;
			m_UniformMVPBuffer = nullptr;
			m_UniformUtilityBuffer = nullptr;
			m_GeometryBuffer = nullptr;
			m_DeferredRenderer = nullptr;
			m_ResourceManager = nullptr;
		}

		GLRenderer::~GLRenderer()
		{
			delete m_UniformMVPBuffer;
			delete m_UniformUtilityBuffer;
			delete m_GeometryBuffer;
			delete m_DeferredRenderer;
			delete m_CurrentContext;
			delete m_ResourceManager;

			for (auto& re : m_RenderEntities)
			{
				delete re.second;
			}
		}

		bool GLRenderer::Initialize(CrRendererContext& a_Context)
		{
			m_CurrentContext = new GLContext();
			m_Window = a_Context.targetWindow;

			if (!m_CurrentContext->CreateContext(m_Window->GetHandle()))
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

#ifdef CR_GRAPHICS_DEBUG
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


		void GLRenderer::Render(Scene::CrScene* a_Scene)
		{
			/*
			Deferred Pipeline:
			1) Geometry Pass: Renders geometry into FBO
			2) Lighting Pass: Applies Lighting to geometry
			3) Additional Passes: Shadows, Reflections, PostFX
			*/

			auto renderList = a_Scene->GetRenderList();
			for (auto& renderable : renderList)
			{
				auto it = m_RenderEntities.find(renderable);

				if (it != m_RenderEntities.end())
					it->second->Render();
				else
					this->CreateEntity(renderable);
			}

			auto camNode = a_Scene->GetNode<Scene::CrCameraNode>();

			MVP mvp{};
			mvp.modelMatrix = glm::mat4(0);
			mvp.projectionMatrix = camNode->GetProjectionMatrix();
			mvp.viewMatrix = camNode->GetViewMatrix();
			m_UniformMVPBuffer->Subdata(&mvp,0);

			ShaderUtil util{};
			util.globalTime = SEngine->GetGameTimer()->GetElapsed<float_t>();
			m_UniformUtilityBuffer->Subdata(&util,0);

			m_GeometryBuffer->Bind();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				for (auto& n : m_RenderEntities)
				{
					n.second->Render();
				}
			m_GeometryBuffer->Unbind();

			m_DeferredRenderer->Render(a_Scene);
			m_CurrentContext->SwapBuffer();
		}

		GLUniformBuffer<MVP>* GLRenderer::GetMVPBuffer() const
		{
			return m_UniformMVPBuffer;
		}

		GLUniformBuffer<ShaderUtil>* GLRenderer::GetUtilBuffer() const
		{
			return m_UniformUtilityBuffer;
		}

		void GLRenderer::CreateEntity(IRenderable * a_Renderable)
		{
			auto mesh = a_Renderable->GetMesh();
			auto material = a_Renderable->GetMaterial();
			auto mode = a_Renderable->GetRenderMode();
			auto boundingBox = a_Renderable->GetBoundingBox();
			auto transform = a_Renderable->GetTransform();

			GLVertexArray* vao = new GLVertexArray(mesh);
			GLShaderProgram* program = new GLShaderProgram(material);
			GLenum glmode = m_RenderModeMappings[mode];
	
			m_RenderEntities[a_Renderable] = new GLRenderEntity(vao, program, glmode, transform, boundingBox);
		}

	}
}