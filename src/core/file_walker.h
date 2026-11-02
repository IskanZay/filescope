#pragma once
#include <filesystem>
#include <vector>

namespace filescope
{
	std::vector<std::filesystem::path> walk(const std::vector<std::filesystem::path>& inputs, bool recursive);
}