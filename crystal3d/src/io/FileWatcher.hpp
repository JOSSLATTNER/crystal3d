#pragma once
#include "core\Core.h"

namespace IO
{
	class CrDirectoryWatcher
	{
		CrDirectoryWatcher(const IO::CrPath& a_Directory)
			: m_Directory(a_Directory)
		{
			LPCWSTR basePath = a_Directory.c_str();
			m_WatchHandle = FindFirstChangeNotificationW
			(
				basePath,
				FALSE,
				FILE_NOTIFY_CHANGE_LAST_WRITE
			);

			if (m_WatchHandle == INVALID_HANDLE_VALUE)
				throw CrException("FindFirstChangeNotification() failed.");

			HANDLE poolHandle;
			if (!RegisterWaitForSingleObject(&poolHandle,
				m_WatchHandle, Callback, m_WatchHandle, INFINITE, WT_EXECUTEINWAITTHREAD))
					throw CrException("RegisterWaitForSingleObject() failed.");

		}

		~CrDirectoryWatcher()
		{
			FindCloseChangeNotification(m_WatchHandle);
		}

		static void __stdcall Callback(void* args, BOOLEAN timeout)
		{
			HANDLE handle = reinterpret_cast<HANDLE>(args);



			FindNextChangeNotification(handle);
		}

	private:
		HANDLE m_WatchHandle;
		IO::CrPath m_Directory;
	};
}