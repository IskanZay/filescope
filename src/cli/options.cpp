#include "options.h"

#include <iostream>
#include <stdexcept>
#include <string_view>

namespace filescope::cli
{
	namespace
	{
		constexpr std::string_view kVersion = "0.1.0";

		bool is_flag(std::string_view arg)
		{
			return arg.size() >= 2 && arg[0] == '-';
		}

		std::pair<std::string, std::string> split_eq(std::string_view arg)
		{
			int minus_pos = 2;
			int eq_pos = arg.find('=');
			if (eq_pos == std::string::npos)
			{
				return { std::string(arg), std::string() };
			}

			return { std::string(arg.substr(0, eq_pos)), std::string(arg.substr(eq_pos + 1)) };
		}
	}


	Options parse_argv(int argc, char** argv)
	{
		Options opts;
		//filescope scan ./samples --recursive --format=json
		for (int i = 2; i < argc; i++)
		{
			auto [name, value] = split_eq(argv[i]);

			if (name == "--help")
			{
				print_help();
				return {};
			}

			if (name == "--version")
			{
				print_version();
				return {};
			}

			if (name == "--recursive")
			{
				opts.recursive = true;
				continue;
			}

			if (name == "--verbose")
			{
				opts.verbose = true;
				continue;
			}

			if (name == "--format")
			{
				if (value.empty())
				{
					if (i + 1 >= argc) throw std::runtime_error("--format requires a value");
					value = argv[++i];
				}
				if (value != "table" && value != "json")
				{
					throw std::runtime_error("--format must be <table> or <json>");
				}
				opts.format = value;
				continue;
			}

			if (name == "scan")
			{
				opts.command = Command::Scan;
				continue;
			}
			else if (name == "inspect")
			{
				opts.command = Command::Inspect;
				continue;
			}
			else if (name == "identify")
			{
				opts.command = Command::Identify;
				continue;
			}

			if (name.find("./") != std::string::npos)
			{
				opts.paths.emplace_back(argv[i]);
				continue;
			}

			if (!is_flag(name)) throw std::runtime_error("unknown flag");
		}

		return opts;
	}

	void print_help()
	{
		std::cout << R"(filescope instruction:
first argument - comamnd
second argument - path
optional arguments: --recursion - recursion traverse,
--format - json or table
Example of full command: filescope scan ./samples --recursive --format=json)";
	}

	void print_version()
	{
		std::cout << kVersion;
	}
}