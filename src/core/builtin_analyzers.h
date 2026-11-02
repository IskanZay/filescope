#pragma once
#include "core/AnalyzerRegistry.h"
#include <memory>

namespace filescope
{
	void register_builtin_analyzers(AnalyzerRegistry& reg);
}