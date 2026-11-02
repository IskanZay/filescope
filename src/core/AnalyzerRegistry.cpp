#include "core/AnalyzerRegistry.h"

void AnalyzerRegistry::registerFactory(std::string name, Factory f)
{
	factories_.emplace(std::move(name), std::move(f));
}

std::vector<std::unique_ptr<IAnalyzer>> AnalyzerRegistry::create_applicable(std::span<const std::byte> data)
{
	std::vector<std::unique_ptr<IAnalyzer>> out;

	for (auto& [name, f] : factories_)
	{
		auto a = f();
		if (a->can_handle(data)) 
		{
			out.push_back(std::move(a));
		}
	}

	return out;
}

std::vector<std::string> AnalyzerRegistry::names() const
{
	std::vector<std::string> out;

	for (auto& [name, _] : factories_)
	{
		out.push_back(name);
	}

	return out;
}