#include "Engine.h"

#include "graphics\opengl\GLRenderer.h"

#ifdef Cr_WINDOWS
#include "window\win32\Win32Window.h"
#include "input\xinput\XIInputManager.h"
#endif

namespace Core
{

	const uint32_t FPS_NO_CAP = UINT32_MAX;
	const uint32_t MAX_FPS = 120U;

	CrEngine::CrEngine()
		: m_MainWindow(nullptr), m_ActiveScene(nullptr), m_GameTimer(nullptr), m_ResourceManager(nullptr),
		m_InputManager(nullptr), m_Renderer(nullptr)
	{
	}

	CrEngine::~CrEngine()
	{
		delete m_MainWindow;
		delete m_ActiveScene;
		delete m_GameTimer;
		delete m_ResourceManager;
		delete m_InputManager;
		delete m_Renderer;
	}

	bool CrEngine::Initialize()
	{

#ifdef Cr_WINDOWS
		m_MainWindow = new Window::Windows32::Win32Window();
		m_InputManager = new Input::XInput::XIInputManager();
#endif

		m_GameTimer = new Core::CrGameTimer();
		m_ResourceManager = new Resources::CrResourceManager();

		Window::CrWindowContext windowContext{};
		windowContext.title = "Engine";
		windowContext.width = 1920;
		windowContext.height = 1080;
		windowContext.fullscreen = false;

		if (!m_MainWindow->Initialize(windowContext))
		{
			CrAssert(0, "Window failed to initialize.");
			return false;
		}

		m_MainWindow->OnClose(std::bind(&CrEngine::Quit, this));

		m_Renderer = new Graphics::OpenGL::GLRenderer();

		Graphics::CrRendererContext rendererContext{};
		rendererContext.targetWindow = m_MainWindow;
		rendererContext.viewportHeight = windowContext.height;
		rendererContext.viewportWidth = windowContext.width;

		if (!m_Renderer->Initialize(rendererContext))
		{
			CrAssert(0, "Renderer failed to initialize.");
			return false;
		}

		return true;
	}

	void CrEngine::Run()
	{
		m_MainWindow->Show();
		m_IsRunning = true;

		while (m_IsRunning)
		{
			float_t delta = m_GameTimer->GetDelta<float_t>();

			if (delta >= 1.0 / MAX_FPS)
			{
				//move windows specific stuff
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
		if (a_GameScene == nullptr)
		{
			CrAssert(0, "Scene was nullptr!");
			return;
		}

		m_ActiveScene = a_GameScene;
		m_ActiveScene->Initialize();
	}

	void CrEngine::Quit()
	{
		CrLog("Shutting down...");
		m_IsRunning = false;
	}

	Window::IWindow * CrEngine::GetMainWindow()
	{
		return m_MainWindow;
	}

	Scene::CrScene * CrEngine::GetCurrentScene()
	{
		return m_ActiveScene;
	}

	Core::CrGameTimer * CrEngine::GetGameTimer()
	{
		return m_GameTimer;
	}

	Resources::CrResourceManager * CrEngine::GetResourceManager()
	{
		return m_ResourceManager;
	}

	Input::IInputManager * CrEngine::GetInputManager()
	{
		return m_InputManager;
	}

	Graphics::IRenderer * CrEngine::GetRenderer()
	{
		return m_Renderer;
	}

	void CrEngine::Render()
	{
		if (m_ActiveScene != nullptr)
		{
			m_Renderer->Render(m_ActiveScene);
		}
	}

	void CrEngine::Update(float& delta) const
	{
		if (m_ActiveScene != nullptr)
		{
			m_ActiveScene->Update(delta);
		}
	}
}