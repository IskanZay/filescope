#include <string>
#include <span>

#include "elf_analyzer.h"
#include "core/BinaryReader.h"
#include "core/FileBuffer.h"

namespace filescope
{
	namespace
	{
		constexpr std::byte f7 =   { 0x7f };
		constexpr std::byte elf0 = { 'E' };
		constexpr std::byte elf1 = { 'L' };
		constexpr std::byte elf2 = { 'F' };
	}

	bool filescope::ElfAnalyzer::can_handle(std::span<const std::byte> data) const noexcept
	{
		if (data.size() >= 4 && data[0] == f7 && data[1] == elf0
			&& data[2] == elf1 && data[3] == elf2) { return true; }
		return false;
	}

	AnalysisResult filescope::ElfAnalyzer::analyze(std::span<const std::byte> data)
	{
		BinaryReader reader(data);
		AnalysisResult r;
		r.analyzer_name = "elf";
	}

}
