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
        for (int i = 0; i < argc; i++)
        {
            auto [name, value] = split_eq(argv[i]);

            if (is_flag(name))
            {
                if (name == "--help" || name == "-h")
                {
                    opts.command = Command::Help;
                    return opts;
                }
                if (name == "--version" || name == "-V")
                {
                    opts.command = Command::Version;
                    return opts;
                }
                if (name == "--recursive" || name == "-r")
                {
                    opts.recursive = true;
                    continue;
                }
                if (name == "--verbose" || name == "-v")
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

                throw std::runtime_error("unknown flag: " + name);
            }

            if (name == "scan") { opts.command = Command::Scan; continue; }
            if (name == "inspect") { opts.command = Command::Inspect; continue; }
            if (name == "identify") { opts.command = Command::Identify; continue; }

            opts.paths.emplace_back(argv[i]);
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