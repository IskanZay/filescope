#include <iostream>
#include <iomanip>

#include "cli/output/table_printer.h"

namespace filescope
{
    std::string filescope::compress_path(std::string_view path)
    {
        if (path.empty()) return "-";

        if (path.length() <= 32)
        {
            return std::string(path);
        }

        std::size_t last_slash = path.rfind('\\');
        std::size_t last_slash_forward = path.rfind('/');

        if (last_slash == std::string_view::npos ||
            (last_slash_forward != std::string_view::npos && last_slash_forward > last_slash))
        {
            last_slash = last_slash_forward;
        }

        if (last_slash == std::string_view::npos)
        {
            return std::string(path.substr(0, 32)) + "...";
        }

        std::string filename = std::string(path.substr(last_slash));

        std::size_t start_len = 32 - filename.length() - 3;
        if (start_len < 3) start_len = 3;

        return std::string(path.substr(0, start_len)) + "..." + filename;
    }

    void filescope::print_table(const std::vector<FileReport>& reports)
    {
        std::cout << std::left << std::setw(40) << "Path" << std::setw(12) << "Size"
            << std::setw(10) << "Format" << std::setw(8) << "Arch" << std::setw(12)
            << "Entropy" << std::setw(12) << "Warnings\n" << std::setw(12) << "Hash";

        for (const auto& r : reports)
        {
            std::string path_str = compress_path(r.path.string());

            std::string arch = "-";
            std::string entropy = "-";
            std::string hash_str = "-";

            for (auto a : r.analyses)
            {
                if (a.analyzer_name == "pe")
                {
                    auto it = a.metadata.find("arch");
                    if (it != a.metadata.end()) arch = it->second;
                }

                if (a.analyzer_name == "entropy")
                {
                    auto it = a.metadata.find("entropy");
                    if (it != a.metadata.end()) entropy = it->second;
                }

                if (a.analyzer_name == "hash") 
                {
                    auto it = a.metadata.find("sha256");
                    if (it != a.metadata.end() && it->second.length() >= 8) 
                    {
                        hash_str = it->second.substr(0, 8); 
                    }
                }
            }

            std::cout << std::left << std::setw(40) << path_str << std::setw(12) << r.size
                << std::setw(10) << (r.format.empty() ? "-" : r.format) << std::setw(8) 
                << arch << std::setw(12) << entropy << std::setw(12) << hash_str << std::setw(12);

            if (!r.warnings.empty())
            {
                for (size_t i = 0; i < r.warnings.size(); i++)
                {
                    if (i > 0) std::cout << "; ";
                    std::cout << r.warnings[i];
                }
            }
            else
            {
                std::cout << "-";
            }

            std::cout << "\n";
        }
    }
}
