#include <iostream>
#include <iomanip>

#include "cli/output/table_printer.h"

void filescope::print_table(const std::vector<FileReport>& reports)
{
	std::cout << std::left << std::setw(40) << "Path" << std::setw(40) 
		<< "Size" << std::setw(40) << "Format" << std::setw(40) << "Warnings";

	for (const auto& r : reports)
	{
		std::cout << std::left << std::setw(40) << r.path << std::setw(40)
			<< r.size << std::setw(40) << r.format;
		for (const auto& warning : r.warnings)
		{
			std::cout << std::setw(40) << warning << ' ';
		}
	}
}
