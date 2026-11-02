#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include <span>
#include <string> 

#include "core/IAnalyzer.h"

using Factory = std::function<std::unique_ptr<IAnalyzer>()>;

class AnalyzerRegistry
{
private:
	std::unordered_map<std::string, Factory> factories_;

public:
	void registerFactory(std::string name, Factory f);
	std::vector<std::unique_ptr<IAnalyzer>> create_applicable(std::span<const std::byte> data);
	std::vector<std::string> names() const;
};