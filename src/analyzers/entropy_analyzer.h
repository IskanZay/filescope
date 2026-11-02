#pragma once
#include <span>

#include "core/IAnalyzer.h"

namespace filescope
{
	class EntropyAnalyzer : public IAnalyzer
	{
	public:

		std::string name() const { return "entropy"; }

		bool can_handle() const { return true; }

		void analyze(std::span<const std::byte> data);
	};
}