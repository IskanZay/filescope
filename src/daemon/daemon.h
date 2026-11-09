#pragma once
#include <string>

namespace filescope
{
	void run_daemon();

	std::string analyze_file_json(const std::string& filepath);
}