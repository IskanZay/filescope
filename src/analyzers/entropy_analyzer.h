#pragma once
#include <span>
#include <string_view>

#include "core/IAnalyzer.h"

namespace filescope
{
	class EntropyAnalyzer : public IAnalyzer
	{
	public:

		std::string_view name() const noexcept override { return "entropy"; }

		bool can_handle(std::span < const std::byte> data) const noexcept { return true; }

		AnalysisResult analyze(std::span<const std::byte> data) override;
	};
}