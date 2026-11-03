#include <iostream>
#include <exception>
#include <vector>
#include <windows.h>

#include "cli/options.h"
#include "cli/runner.h"

int main()
{
    /*SetConsoleCP(1251);
    SetConsoleOutputCP(1251);*/

    try
    {
        int argc;
        std::cin >> argc;
        std::cin.ignore();

        std::vector<std::string> args_storage;
        args_storage.push_back("filescope.exe");

        for (int i = 1; i < argc; i++)
        {
            std::string cur;
            std::getline(std::cin, cur);
            args_storage.push_back(cur);
        }

        std::vector<char*> raw_argv;
        for (auto& s : args_storage)
        {
            raw_argv.push_back(s.data());
        }

        filescope::cli::Runner runner;
        runner.run(filescope::cli::parse_argv(static_cast<int>(raw_argv.size()), raw_argv.data()));
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }

    return 0;
}