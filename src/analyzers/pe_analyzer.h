#pragma once
#include "core/IAnalyzer.h"
#include <string_view>
#include <span>

namespace filescope
{
	class PeAnalyzer : public IAnalyzer
	{
	public:
		std::string_view name() const noexcept override { return "pe"; }
		bool can_handle(std::span<const std::byte> data) const noexcept;
		AnalysisResult analyze(std::span<const std::byte> data) override;
	};
}