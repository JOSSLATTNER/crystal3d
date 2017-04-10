#include "GLRenderer.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLUniformBuffer<MVP>* GLRenderer::MVPBuffer = nullptr;
		GLUniformBuffer<ShaderUtil>* GLRenderer::UtilBuffer = nullptr;

		GLRenderer::GLRenderer()
		{
			m_GeometryBuffer = nullptr;
			m_CurrentContext = nullptr;
		}

		GLRenderer::~GLRenderer()
		{
			delete m_GeometryBuffer;
			delete GLRenderer::MVPBuffer;
			delete GLRenderer::UtilBuffer;

			for (auto& re : m_RenderEntities)
				delete re.second;

			GLCache::Clear();
			delete m_CurrentContext;
		}

		void GLRenderer::Initialize(CrRendererContext& a_Context)
		{
			//Create Context
			m_Window = a_Context.targetWindow;
			m_CurrentContext = new GLContext(m_Window->GetHandle());

			//@GL Settings
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_TEXTURE_2D);
			glDepthFunc(GL_LESS); 
			glClearColor(0., 0., 0., 1.);
			glViewport(0, 0, a_Context.viewportWidth, a_Context.viewportHeight);

#ifdef CR_GRAPHICS_DEBUG
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
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

			//Uniform Buffer
			GLRenderer::MVPBuffer = new GLUniformBuffer<MVP>("MVPBuffer", GL_DYNAMIC_DRAW);
			GLRenderer::UtilBuffer = new GLUniformBuffer<ShaderUtil>("UtilBuffer", GL_DYNAMIC_DRAW);

			//Deferred Renderer
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
			auto camNode = a_Scene->GetNode<Scene::CrCameraNode>();

			MVP mvp{};
			mvp.modelMatrix = glm::mat4(0);
			mvp.projectionMatrix = camNode->GetProjectionMatrix();
			mvp.viewMatrix = camNode->GetViewMatrix();
			GLRenderer::MVPBuffer->Subdata(&mvp,0);

			ShaderUtil util{};
			util.globalTime = SEngine->GetGameTimer()->GetTotal<float_t>();
			GLRenderer::UtilBuffer->Subdata(&util,0);

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

		void GLRenderer::LoadAssets(Scene::CrScene * a_Scene)
		{
			auto renderList = a_Scene->GetRenderList();
			for (auto& renderable : renderList)
				this->CreateEntity(renderable);
			CrLogSuccess("Assets loaded!");
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
			GLRenderer::MVPBuffer->Bind(program->GetHandle());

			GLenum glmode = m_RenderModeMappings[mode];
	
			GLRenderEntity* entity = new GLRenderEntity(vao, program);
			entity->SetMode(glmode);
			entity->SetBoundingBox(boundingBox);
			entity->SetTransform(transform);

			m_RenderEntities[a_Renderable] = entity;
		}

	}
}