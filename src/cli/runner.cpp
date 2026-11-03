#include <iostream>

#include "cli/runner.h"
#include "cli/output/table_printer.h"
#include "core/AnalyzerRegistry.h"
#include "core/builtin_analyzers.h"
#include "core/file_walker.h"
#include "core/FileBuffer.h"
#include "core/result_store.h"

namespace filescope::cli 
{
    int Runner::run(const Options& opts) 
    {
        if (opts.command == Command::Help || opts.command == Command::Version) 
        {
            return 0;
        }

        AnalyzerRegistry reg;
        register_builtin_analyzers(reg); 

        auto files = walk(opts.paths, opts.recursive);
        if (files.empty()) 
        {
            std::cerr << "No files found\n";
            return 2;
        }

        ResultStore store;
        for (const auto& file : files) 
        {
            FileReport rep;
            rep.path = file;

            try 
            {
                FileBuffer buf(file);
                rep.size = buf.size();
                auto analyzers = reg.create_applicable(buf.bytes());

                for (auto& analyzer : analyzers) 
                {
                    auto result = analyzer->analyze(buf.bytes());

                    if (!result.format.empty()) 
                    {
                        rep.format = result.format;
                    }

                    for (const auto& w : result.warnings) 
                    {
                        rep.warnings.push_back(w);
                    }
                }
            }
            catch (const std::exception& e) 
            {
                rep.warnings.push_back(e.what());
            }

            store.add(std::move(rep)); 
        }

        auto results = store.snapshot();
        print_table(results);

        return 0;
    }

} 