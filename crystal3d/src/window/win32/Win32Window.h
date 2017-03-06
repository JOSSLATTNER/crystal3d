#pragma once
#include "window\interface\IWindow.h"
#include "core\Platform.h"

#define CR_WIN32_CLASS_NAME "Crystal3D"

namespace Window
{
	namespace Windows32
	{
		class Win32Window 
			: public IWindow
		{
		public:
			Win32Window();

			//Inherited via IWindow
			~Win32Window() override;
			bool Initialize(CrWindowContext& a_WindowContext) override;
			CrWindowHandle GetHandle() override;

			void Show() override;
			void SetTitle(const std::string& a_Ttitle) override;
			glm::vec2 GetSize() const override;

			virtual void OnClose(std::function<void()> a_Callback) override;
			virtual void OnFocus(std::function<void()> a_Callback) override;
			virtual void OnResize(std::function<void()> a_Callback) override;

			//WndProc
			static LRESULT CALLBACK WndProc(HWND a_Hwnd, unsigned int a_Message, WPARAM a_WParam, LPARAM a_LPARAM);

		protected:
			HWND m_Handle;
			HINSTANCE m_HInstance;
			std::string m_ClassName;
			std::multimap<UINT, std::function<void()>> m_WindowCallbacks;

		};
	}
}