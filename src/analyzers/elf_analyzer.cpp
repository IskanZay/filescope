#include <string>
#include <span>

#include "elf_analyzer.h"
#include "core/BinaryReader.h"
#include "core/FileBuffer.h"

namespace filescope
{
	namespace
	{
		constexpr std::byte f7  { 0x7f };
		constexpr std::byte elf0{ 'E' };
		constexpr std::byte elf1{ 'L' };
		constexpr std::byte elf2{ 'F' };
	}

	bool ElfAnalyzer::can_handle(std::span<const std::byte> data) const noexcept
	{
		if (data.size() >= 4 && data[0] == f7 && data[1] == elf0
			&& data[2] == elf1 && data[3] == elf2) { return true; }
		return false;
	}

	AnalysisResult ElfAnalyzer::analyze(std::span<const std::byte> data)
	{
		BinaryReader reader(data);
		AnalysisResult r;
		r.analyzer_name = "elf";

		auto EI_CLASS = reader.read_at<uint8_t>(4);
		if (EI_CLASS && *EI_CLASS == 1)
		{
			r.metadata["arch"] = "x32";
		}
		else
		{
			r.metadata["arch"] = "x64";
		}

		auto EI_DATA = reader.read_at<uint8_t>(5);
		if (EI_DATA && *EI_DATA == 1)
		{
			r.metadata["endianness"] = "Little-endian";
		}
		else if (EI_DATA && *EI_DATA == 2)
		{
			r.metadata["endianness"] = "Big-endian";
		}

		//	1 = .o (объектный). 2 = Исполняемый. 3 = Библиотека .so. 4 = Core dump.
		auto file_type = reader.read_at<uint16_t>(16);
		if (file_type && *file_type == 1)
		{
			r.metadata["file type"] = ".o";
		}
		else if (file_type &&  *file_type == 2)
		{
			r.metadata["file type"] = "executable";
		}
		else if (file_type &&  *file_type == 3)
		{
			r.metadata["file type"] = ".so";
		}
		else if (file_type && *file_type == 4)
		{
			r.metadata["file type"] = "core dump";
		}

		//3 = x86. 62 = x86-64. 40 = ARM. 183 = AArch64 (ARM64).
		auto arch = reader.read_at<uint16_t>(18);
		if (arch && *arch == 3)
		{
			r.metadata["arch"] = "x86";
		}
		else if (arch && *arch == 62)
		{
			r.metadata["arch"] = "x86-64";
		}
		else if (arch && *arch == 40)
		{
			r.metadata["arch"] = "ARM";
		}
		else if (arch && *arch == 183)
		{
			r.metadata["arch"] = "AArch64 (ARM64)";
		}

		return r;
	}

}
