#pragma once
#include <span>
#include <string_view>
#include <cmath>
#include <cstdio>

#include "core/IAnalyzer.h"

namespace filescope
{
	class EntropyAnalyzer : public IAnalyzer
	{
	public:

		std::string_view name() const noexcept override { return "entropy"; }

		bool can_handle(std::span < const std::byte> data) const noexcept override;

		AnalysisResult analyze(std::span<const std::byte> data) override;
	};
}