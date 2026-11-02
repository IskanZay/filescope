#include "core/file_walker.h"

namespace filescope
{
    std::vector<std::filesystem::path> walk(const std::vector<std::filesystem::path>& inputs, bool recursive)
    {
        std::vector<std::filesystem::path> out;

        for (const auto& p : inputs)
        {
            std::error_code ec;

            if (std::filesystem::is_regular_file(p, ec))
            {
                out.push_back(p);
            }
            else if (std::filesystem::is_directory(p, ec))
            {
                if (recursive)
                {
                    for (auto it = std::filesystem::recursive_directory_iterator(p, ec); !ec && it != std::filesystem::recursive_directory_iterator(); it++)
                    {
                        if (it->is_regular_file(ec))
                        {
                            out.push_back(it->path());
                        }
                    }
                }
                else
                {
                    for (auto it = std::filesystem::directory_iterator(p, ec); !ec && it != std::filesystem::directory_iterator(); it++)
                    {
                        if (it->is_regular_file(ec))
                        {
                            out.push_back(it->path());
                        }
                    }
                }
            }
        }

        return out;
    }
}