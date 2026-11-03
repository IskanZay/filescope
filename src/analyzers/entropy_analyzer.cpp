#pragma once
#include <span>
#include <array>

#include "core/AnalyzerRegistry.h"
#include "core/IAnalyzer.h"
#include "analyzers/entropy_analyzer.h"

namespace filescope
{
	bool EntropyAnalyzer::can_handle(std::span <const std::byte> data) const noexcept { return true; }

	AnalysisResult EntropyAnalyzer::analyze(std::span<const std::byte> data)
	{
		AnalysisResult r;
		r.analyzer_name = "entropy";

		std::array<std::size_t, 256> bytes;

		for (auto b : data)
		{
			bytes[static_cast<unsigned char>(b)]++;
		}

		double entrop = 0.0;
		for (auto cnt : bytes)
		{
			if (cnt > 0)
			{
				double p = cnt / data.size();
				entrop -= p * std::log2(p);
			}
		}
		entropy = entrop;

		r.metadata["entropy"] = entropy;
		if (entropy > 7.0) r.warnings.push_back("High entropy: Suspisious file");

		return r;
	}

}