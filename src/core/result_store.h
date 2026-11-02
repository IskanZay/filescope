#pragma once
#include <filesystem>
#include <iostream>
#include <vector>
#include <mutex>

namespace filescope
{

	struct FileReport
	{
		std::filesystem::path path;
		uintmax_t size;
		std::string format;
		std::vector<std::string> warnings;
	};

	class ResultStore
	{
	private:
		std::vector<FileReport> reports;
		mutable std::mutex mtx_;

	public:
		void add(FileReport report)
		{
			std::unique_lock<std::mutex> lock(mtx_);
			reports.push_back(report);
		}

		std::vector<FileReport> snapshot() const
		{
			std::unique_lock<std::mutex> lock(mtx_);
			return reports;
		}
	};
}
