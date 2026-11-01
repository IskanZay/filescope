#include "AnalyzerRegistry.h"
#include "magic_analyzer.h"
#include <memory>

namespace filescope
{
	void register_builtin_analyzers(AnalyzerRegistry& reg)
	{
		reg.registerFactory("magic", []() {return std::make_unique<MagicAnalyzer>(); });
	}
}