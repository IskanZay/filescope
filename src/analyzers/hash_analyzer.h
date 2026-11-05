#pragma once
#include <span>
#include <string_view>
#include "core/IAnalyzer.h"

namespace filescope 
{
    class HashAnalyzer : public IAnalyzer 
    {
    public:
        std::string_view name() const noexcept override { return "hash"; }
        bool can_handle(std::span<const std::byte> data) const noexcept override;
        AnalysisResult analyze(std::span<const std::byte> data) override;
    };
}