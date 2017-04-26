#pragma once
#include "core\Core.h"

namespace IO
{
	typedef std::function<void(std::wstring)> CrIONotification;

	class CrDirectoryWatcher
	{
	public:
		CrDirectoryWatcher()
			: m_IsReading(false)
		{
		}

		~CrDirectoryWatcher()
		{
			this->StopRead();
			CloseHandle(m_hDIR);
		}

		void SetWatch(const IO::CrPath& a_Directory, CrIONotification&& a_Callback, bool a_Recursive = false)
		{
			//Stop current reading thread (if possible)
			this->StopRead();

			//Open directory handle
			LPCWSTR basePath = a_Directory.c_str();
			m_hDIR = CreateFileW(
				basePath,
				FILE_LIST_DIRECTORY,
				FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_BACKUP_SEMANTICS,
				NULL);

			if (m_hDIR == INVALID_HANDLE_VALUE)
				throw CrException(CrWin32ErrorString());

			//Start listening in background thread
			m_IsReading = true;
			m_Callback = std::move(a_Callback);
			m_ReadThread = std::thread(&CrDirectoryWatcher::StartRead, this, a_Recursive);
		}

	private:
		void StopRead()
		{
			m_IsReading = false;
			if (m_ReadThread.joinable())
				m_ReadThread.join();
		}

		void StartRead(bool a_Recursive)
		{
			DWORD dwBytesReturned = 0;
			FILE_NOTIFY_INFORMATION fni[1024];

			while(m_IsReading)
			{
				if (!ReadDirectoryChangesW(m_hDIR,
					&fni, sizeof(fni),
					a_Recursive, FILE_NOTIFY_CHANGE_LAST_WRITE,
					&dwBytesReturned, NULL, NULL))
				{
					throw CrException(CrWin32ErrorString());
				}

				std::wstring fileName(fni[0].FileName, fni[0].FileNameLength);
				m_Callback(fileName);
			}
		}


	private:
		std::atomic<bool> m_IsReading;
		std::thread m_ReadThread;

		//Since no concurrent access is happening on these variables
		//we don't need any synchronization (i.e. mutex||atomic)
		CrIONotification m_Callback;
		HANDLE m_hDIR;
	};
}