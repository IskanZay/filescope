#pragma once
#include <iostream>
#include <string_view>
#include <span>

#include "IAnalyzer.h"

namespace filescope
{

	class MagicAnalyzer : public IAnalyzer
	{
	public:
		std::string_view name() const noexcept override { return "magic"; }

		bool can_handle(std::span<const std::byte> data) const noexcept;

		AnalysisResult analyze(std::span<const std::byte> data) override;
	};
}