#pragma once
#include <span>
#include <array>

#include "core/AnalyzerRegistry.h"
#include "core/IAnalyzer.h"


namespace filescope
{
	class EntropyAnalyzer : public IAnalyzer
	{
		std::size_t entropy;
	public:
		bool can_handle(std::span < const std::byte> data) const noexcept { return true; }

		AnalisysResult analyze(std::span<const std::byte> data)
		{
			AnalysisResult r;
			r.analyzer_name = "entropy";

			std::array<unsigned char, 256> bytes;

			for (auto b : data)
			{
				bytes[static_cast<unsigned char>(b)]++;
			}

			std::size_t entrop;
			for (auto b : bytes)
			{
				entrop -= b * std::log2(b);
			}
			entropy = entrop;

			r.metadata["entropy"] = entropy;
			double res = entropy / data.size();
			if (res > 7.0) r.warnings.push_back("Suspisious file");

			return r;
		}
	};
}