#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <span>


struct AnalysisResult
{
	std::string analyzer_name;
	std::string format;
	std::unordered_map<std::string, std::string> metadata;
	std::vector<std::string> warnings;
	std::chrono::microseconds duration{};
};

class IAnalyzer
{
public:
	virtual ~IAnalyzer() = default;

	virtual std::string_view name() const noexcept = 0;

	virtual bool can_handle(std::span<const std::byte> data) const = 0;

	virtual AnalysisResult analyze(std::span<const std::byte> data) = 0;
};

