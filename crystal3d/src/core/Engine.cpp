#include "Engine.h"
#include "graphics\opengl\GLRenderer.h"

#ifdef CR_PLATFORM_WINDOWS
#include "window\win32\Win32Window.h"
#include "input\xinput\XIInputManager.h"
#endif

Core::CrEngine* Core::CrEngine::s_SharedInstance = nullptr;

namespace Core
{
	CrEngine::CrEngine()
		: m_IsRunning(false),
		m_MainWindow(nullptr),
		m_ActiveScene(nullptr),
		m_GameTimer(nullptr),
		m_InputManager(nullptr),
		m_Renderer(nullptr),
		m_Config("Engine.ini")
	{
		CrEngine::s_SharedInstance = this;
	}

	CrEngine::~CrEngine()
	{
		delete m_MainWindow;
		delete m_ActiveScene;
		delete m_GameTimer;
		delete m_InputManager;
		delete m_Renderer;
		delete m_ResourceManager;
	}

	void CrEngine::Initialize()
	{
#ifdef CR_PLATFORM_WINDOWS
		m_MainWindow = new Window::Windows32::Win32Window();
		m_InputManager = new Input::XInput::XIInputManager();
#endif
		m_GameTimer = new Core::CrGameTimer();
		m_Renderer = new Graphics::OpenGL::GLRenderer();
		m_ResourceManager = new Resources::CrResourceManager();

		//Initialize Window
		Window::CrWindowContext windowContext{};
		windowContext.title = "Sandbox";
		windowContext.width = 1920;
		windowContext.height = 1080;
		windowContext.fullscreen = false;

		m_MainWindow->Initialize(windowContext);
		m_MainWindow->OnClose(std::bind(&CrEngine::Quit, this));

		//Initialize Input Manager
		m_InputManager->Initialize();

		//Initialize Resources
		m_ResourceManager->Initialize(m_Renderer->CreateFactory());

		//Initialize Renderer
		Graphics::CrRendererContext rendererContext{};
		rendererContext.targetWindow = m_MainWindow;
		rendererContext.viewportHeight = windowContext.height;
		rendererContext.viewportWidth = windowContext.width;

		m_Renderer->Initialize(rendererContext);
	}

	void CrEngine::Run()
	{
		m_MainWindow->Show();
		m_IsRunning = true;
		auto maxFps = m_Config.GetValue<int>("Engine", "MaxFPS");

		while (m_IsRunning)
		{
			const float_t delta = m_GameTimer->GetDelta<float_t>();

			if (delta >= 1 / maxFps)
			{
				//TODO: MOVE WIN32 STUFF
				MSG msg{};
				while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				m_InputManager->Poll();
				this->Update(delta);
				this->Render();

				m_GameTimer->Reset();
			}
		}
	}

	void CrEngine::SetScene(Scene::CrScene* a_GameScene)
	{
		CrAssert(a_GameScene != nullptr, "Scene was nullptr!");

		m_ActiveScene = a_GameScene;
		m_ActiveScene->Initialize();
		m_Renderer->LoadAssets(m_ActiveScene);
	}

	void CrEngine::Quit()
	{
		CrLog("Terminating...");
		m_IsRunning = false;
	}

	Core::CrConfiguration* CrEngine::GetConfiguration()
	{
		return &m_Config;
	}

	Window::IWindow* CrEngine::GetMainWindow()
	{
		return m_MainWindow;
	}

	Scene::CrScene* CrEngine::GetCurrentScene()
	{
		return m_ActiveScene;
	}

	Core::CrGameTimer* CrEngine::GetGameTimer()
	{
		return m_GameTimer;
	}

	Input::IInputManager* CrEngine::GetInputManager()
	{
		return m_InputManager;
	}

	Graphics::IRenderer* CrEngine::GetRenderer()
	{
		return m_Renderer;
	}

	Resources::CrResourceManager * CrEngine::GetResourceManager()
	{
		return m_ResourceManager;
	}

	void CrEngine::Render()
	{
		if (m_ActiveScene != nullptr)
		{
			m_Renderer->Render(m_ActiveScene);
		}
	}

	void CrEngine::Update(const float a_Delta) const
	{
		if (m_ActiveScene != nullptr)
		{
			m_ActiveScene->Update(a_Delta);
		}
	}
}