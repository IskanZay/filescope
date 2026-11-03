#pragma once
#include "core/result_store.h"
#include <vector>
#include <string_view>

namespace filescope 
{
    std::string compress_path(std::string_view path);

    void print_table(const std::vector<FileReport>& reports);
}