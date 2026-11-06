#pragma once

#include "core/IAnalyzer.h"

#include <span>
#include <string_view>

namespace filescope
{
	class ElfAnalyzer : public IAnalyzer
	{
		std::string_view name() const noexcept override { return "elf"; }
		bool can_handle(std::span<const std::byte> data) const noexcept override;
		AnalysisResult analyze(std::span<const std::byte> data) override;
	};
}