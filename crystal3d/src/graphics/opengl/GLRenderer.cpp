#include "GLRenderer.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLRenderer::GLRenderer()
		{
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

			for (auto& re : m_RenderEntities)
			{
				delete re.second;
			}
		}

		void GLRenderer::Initialize(CrRendererContext& a_Context)
		{
			m_Window = a_Context.targetWindow;
			m_CurrentContext.Create(m_Window->GetHandle());

			GLenum err = glewInit();

			if (GLEW_OK != err)
				throw CrException(CrStringFormat("GLEW ERROR: %s", glewGetErrorString(err)));

			CrLogInfo("Version: %s\nVendor: %s\nShader Version: %s",
				glGetString(GL_VERSION), glGetString(GL_VENDOR), glGetString(GL_SHADING_LANGUAGE_VERSION));

			//@GL Settings
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_TEXTURE_2D);
			glDepthFunc(GL_LESS); 
			glClearColor(0., 0., 0., 1.);
			glViewport(0, 0, a_Context.viewportWidth, a_Context.viewportHeight);

#ifdef CR_GRAPHICS_DEBUG
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(GLDebugCallback, NULL);

			GLuint unusedIds = 0;
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
				GL_DONT_CARE, 0, &unusedIds, GL_TRUE);
#endif

			//Geometry Buffer
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

			CrLogSuccess("Renderer initialized [OK]");
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
			m_CurrentContext.SwapBuffer();
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
			m_UniformMVPBuffer->Bind(program->GetHandle());

			GLenum glmode = m_RenderModeMappings[mode];
	
			GLRenderEntity* entity = new GLRenderEntity(vao, program);
			entity->SetMode(glmode);
			entity->SetBoundingBox(boundingBox);
			entity->SetTransform(transform);
			entity->SetTransformBufferFunc([&](glm::mat4& ma)
			{
				m_UniformMVPBuffer->Subdata(&ma);
			});

			m_RenderEntities[a_Renderable] = entity;
		}

	}
}