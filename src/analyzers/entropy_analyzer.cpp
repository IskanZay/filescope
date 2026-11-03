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

        if (data.empty()) 
        {
            r.metadata["entropy"] = "0.00";
            return r;
        }

        std::array<std::size_t, 256> bytes{};

        for (auto b : data)
        {
            bytes[static_cast<unsigned char>(b)]++;
        }

        double entrop = 0.0;
        double total_size = static_cast<double>(data.size());

        for (auto cnt : bytes)
        {
            if (cnt > 0)
            {
                double p = static_cast<double>(cnt) / total_size;
                entrop -= p * std::log2(p);
            }
        }

        char buf[16];
        snprintf(buf, sizeof(buf), "%.2f", entrop);
        r.metadata["entropy"] = buf;

        if (entrop > 7.0) {
            r.warnings.push_back("HIGH_ENTROPY");
        }

        return r;
    }

}

}