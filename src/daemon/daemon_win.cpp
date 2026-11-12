#ifdef _WIN32
#include "daemon.h"
#include <windows.h>
#include <iostream>
#include <string>

namespace filescope
{
	void run_daemon()
	{
		const wchar_t* pipename = L"\\\\.\\pipe\\FileScopePipe";

		cout << "Daemon waiting for connects\n";

		while (true)
		{
			HANDLE pipe = CreateNamedPipeW
			(
				pipe_name,
				PIPE_ACCESS_DUPLEX,
				PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
				1, 4096, 4096, 0, nullptr
			);

			if (pipe == INVALID_HANDLE_VALUE)
			{
				std::cout << "Error to create pipe by daemon";
				return;
			}

			if (ConnectNamedPipe(pipe, nullptr) != false)
			{
				char buffer[1024] = { 0 };
				DWORD bytes_read;

				if (ReadFile(pipe, buffer, sizeof(buffer) - 1, &bytes_read, nullptr))
				{
					std::string filepath(buffer);
					std::string response = analyze_file_json(filepath);

					DWORD bytes_written;
					WriteFile(pipe, response.c_str(), static_cast<DWORD>(response.size()), &bytes_written, nullptr);
				}
				DisconnectNamedPipe(pipe);
			}
			CloseHandle(pipe);
		}
	}
}
#endif