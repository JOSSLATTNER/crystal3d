#include "Win32Window.h"
#include "core\Engine.h"

namespace Window
{
	namespace Windows32
	{

		Win32Window::Win32Window()
			: m_Handle(0), m_HInstance(0)
		{
		}

		Win32Window::~Win32Window()
		{
			DestroyWindow(m_Handle);
			UnregisterClass(m_ClassName.c_str(), m_HInstance);
		}

		bool Win32Window::Initialize(CrWindowContext& a_WindowContext)
		{
			DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

			m_HInstance = GetModuleHandle(0);
			m_ClassName = "FrozenEngine";

			WNDCLASS wndClass = {};
			wndClass.style = CS_HREDRAW | CS_VREDRAW;
			wndClass.lpfnWndProc = WndProc;
			wndClass.hInstance = m_HInstance;
			wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wndClass.lpszClassName = m_ClassName.c_str();

			if (!RegisterClass(&wndClass))
			{
				CrAssert(0, "WIN32 ERROR: %d", GetLastError());
				return false;
			}

			RECT wr = { 0, 0, (LONG)a_WindowContext.width, (LONG)a_WindowContext.height };
			AdjustWindowRect(&wr, style, FALSE);

			m_Handle = CreateWindowEx(NULL,
				m_ClassName.c_str(),
				a_WindowContext.title.c_str(),
				style,
				0,
				0,
				wr.right - wr.left,
				wr.bottom - wr.top,
				NULL,
				NULL,
				m_HInstance,
				this);

			return true;
		}


		void Win32Window::Show()
		{
			SetFocus(m_Handle);
			SetForegroundWindow(m_Handle);
			ShowWindow(m_Handle, SW_SHOW);
		}

		CrWindowHandle Win32Window::GetHandle()
		{
			return m_Handle;
		}

		void Win32Window::SetTitle(const std::string& a_Ttitle)
		{
			SetWindowText(m_Handle, a_Ttitle.c_str());
		}

		LRESULT Win32Window::WndProc(HWND a_Hwnd, unsigned int a_Message, WPARAM a_WParam, LPARAM a_LPARAM)
		{
			Win32Window * thiz = reinterpret_cast<Win32Window*>(GetWindowLongPtr(a_Hwnd, GWLP_USERDATA));

			if (a_Message == WM_CREATE)
			{
				CREATESTRUCT * cs = reinterpret_cast<CREATESTRUCT *>(a_LPARAM);
				SetWindowLongPtr(a_Hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
			}

			if (thiz != nullptr)
			{
				auto matchRange = thiz->m_WindowCallbacks.equal_range(a_Message);

				for (auto it = matchRange.first; it != matchRange.second; ++it)
				{
					it->second();
				}
			}

			return DefWindowProc(a_Hwnd, a_Message, a_WParam, a_LPARAM);
		}

		glm::vec2 Win32Window::GetSize() const
		{
			RECT rect{};
			GetWindowRect(m_Handle, &rect);
			return { rect.right - rect.left , rect.bottom - rect.top };
		}

		void Win32Window::OnClose(std::function<void()> a_Callback)
		{
			m_WindowCallbacks.insert({ WM_CLOSE, a_Callback });
		}

		void Win32Window::OnFocus(std::function<void()> a_Callback)
		{
			m_WindowCallbacks.insert({ WM_SETFOCUS, a_Callback });
		}

		void Win32Window::OnResize(std::function<void()> a_Callback)
		{
			m_WindowCallbacks.insert({ WM_SIZE, a_Callback });
		}

	}
}