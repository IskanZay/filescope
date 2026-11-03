#include <string>

#include "pe_analyzer.h"
#include "core/BinaryReader.h"
#include "core/FileBuffer.h"


namespace filescope
{
	namespace
	{
		constexpr std::byte kMz0{ 'M' };
		constexpr std::byte kMz1{ 'Z' };
	}

	bool PeAnalyzer::can_handle(std::span<const std::byte> data) const noexcept
	{
		if (data.size() >= 2 && data[0] == kMz0 && data[1] == kMz1) return true;
		return false;
	}

	AnalysisResult PeAnalyzer::analyze(std::span<const std::byte> data)
	{
		BinaryReader reader(data);
		AnalysisResult r;

		//Dos заголовок
		auto e_lfanew = reader.read_at<uint32_t>(0x3C);
		if (!e_lfanew)
		{
			r.warnings.push_back("Broken file: no DOS header");
			return r;
		}

		//file header
		auto sig = reader.read_at<uint32_t>(*e_lfanew);
		if (!sig || *sig != 0x00004550)
		{
			r.warnings.push_back("This is not PE file");
			return r;
		}

		//optional header
		auto magic = reader.read_at<uint16_t>(*e_lfanew + 24);
		if (magic)
		{
			if (*magic == 0x10b) r.metadata["arch"] = "x32";
			else if (*magic == 0x20B) r.metadata["arch"] = "x64";
			else r.metadata["arch"] = "unknown";

			r.format = (*magic == 0x20B) ? "PE32+" : "PE32";
		}

		//кол-во секций
		auto num_sections = reader.read_at<uint16_t>(*e_lfanew + 6);

		//Размер заголовка опций
		auto opt_size = reader.read_at<uint16_t>(*e_lfanew + 20);

		if (!num_sections || !opt_size)
		{
			r.warnings.push_back("Broken file: no section headers");
			return r;
		}

		r.metadata["sections_count"] = std::to_string(*num_sections);

		//Начало таблицы секций
		std::uint32_t section_offset = *e_lfanew + 24 + *opt_size;
		for (uint16_t i = 0; i < *num_sections; i++)
		{
			uint32_t current_sec = section_offset + (i * 40); //каждая секция по 40 байт

			auto sec_name_opt = reader.read_cstr(current_sec, 8); //имя секции - перые 8 байт секции

			if (sec_name_opt)
			{
				std::string sec_name(*sec_name_opt);
				if (sec_name.empty()) sec_name = "unnamed";

				r.metadata["sec" + std::to_string(i)] = sec_name;

				if (sec_name == "UPX0" || sec_name == "UPX1" || sec_name == "aspac" || sec_name == ".themida")
				{
					r.warnings.push_back("PACKED" + sec_name);
				}
			}
		}

		return r;
	}
}