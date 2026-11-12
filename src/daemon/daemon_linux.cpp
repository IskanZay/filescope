#ifdef __linux__

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>

namespace filescope
{
	void run_daemon()
	{
		const char* socket_path = "/tmp/filescope.sock";
		unlink(socket_path);

		int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (server_fd == -1)
		{
			std::cerr << "Socket creation failed\n";
			return;
		}

		sockaddr_un addr{};
		addr.sun_family = AF_UNIX;
		strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

		if (::bind(server_fd, (sockaddr*)&addr, sizeof(addr)) == -1)
		{
			std::cerr << "bind failed\n";
			return;
		}

		listen(server_fd, 5);
		std::cout << "Daemon started\n";
		while (true)
		{
			int client_fd = accept(server_fd, nullptr, nullptr);
			if (client_fd >= 0)
			{
				char buffer[1024] = { 0 };
				read(client_fd, buffer, sizeof(buffer) - 1);

				std::string filepath(buffer);
				std::string response = analyze_file_to_json(filepath);

				write(client_fd, response.c_str(), response.size());
				close(client_fd);
			}
		}
	}
}
#endif