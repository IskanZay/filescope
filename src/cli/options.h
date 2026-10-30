#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace filescope::cli 
{
	enum class Command {None, Scan, Inspect, Identify, Help, Version};

	struct Options
	{
		Command command = Command::None;
		std::vector<std::filesystem::path> paths;
		std::string format = "table";
		bool recursive = false;
		bool verbose = false;
	};

	Options parse_argv(int argc, char** argv);

	void print_help();

	void print_version();
}