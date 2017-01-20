#pragma once
#include "core\Core.h"
#include "WindowContext.h"

namespace Window
{
	typedef void* CrWindowHandle;

	class IWindow
	{
	public:
		virtual ~IWindow() {}

		virtual bool Initialize(CrWindowContext& a_WindowContext) = 0;
		virtual void Show() = 0;
		virtual CrWindowHandle GetHandle() = 0;
		virtual void SetTitle(const std::string& a_Title) = 0;

		/*Events*/
		virtual void OnClose(std::function<void()> a_Callback) = 0;
		virtual void OnFocus(std::function<void()> a_Callback) = 0;
		virtual void OnResize(std::function<void()> a_Callback) = 0;

	};
}