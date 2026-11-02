#include "core/builtin_analyzers.h"
#include "analyzers/magic_analyzer.h"
#include "analyzers/entropy_analyzer.h"
#include <memory>

namespace filescope
{
	void register_builtin_analyzers(AnalyzerRegistry& reg)
	{
		reg.registerFactory("magic", []() {return std::make_unique<MagicAnalyzer>(); });
		reg.registerFactory("entropy", []() {return std::make_unique<EntropyAnalyzer()>; });
	}
}