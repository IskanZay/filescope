#include "analyzers/magic_analyzer.h"

namespace filescope
{
	namespace
	{
		constexpr std::byte kMz0{ 'M' };
		constexpr std::byte kMz1{ 'Z' };
		constexpr std::byte kElf0{ 0x7F };
		constexpr std::byte kElf1{ 'E' };
		constexpr std::byte kElf2{ 'L' };
		constexpr std::byte kElf3{ 'F' };
	}

	bool MagicAnalyzer::can_handle(std::span<const std::byte>) const noexcept
	{
		return true;
	}

	AnalysisResult MagicAnalyzer::analyze(std::span<const std::byte> data)
	{
		AnalysisResult r;
		r.analyzer_name = "magic";

		if (data.size() >= 2 && data[0] == kMz0 && data[1] == kMz1) r.format = "PE";
		else if (data.size() >= 4 && data[0] == kElf0 && data[1] == kElf1 &&
			data[2] == kElf2 && data[3] == kElf3) r.format = "ELF";
		else r.format = "unknown";

		r.metadata["format"] = r.format;

		return r;
	}
}