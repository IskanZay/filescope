#include <iostream>
#include <atomic>

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

        auto worker = [&]()
            {
                while (true)
                {
                    size_t i = next_file_id.fetch_add(1);

                    if (i >= files.size()) break;

                    const auto& file = files[i];

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

                            rep.analyses.push_back(result);

                            for (const auto& w : result.warnings)
                            {
                                rep.warnings.push_back(w);
                            }
                        }
                    }
                    catch (const std::exception& e)
                    {
                        rep.warnings.push_back(e.what());
                        rep.format = "ERROR";
                    }

                    store.add(std::move(rep));
                }
            };

        unsigned int num_threads = std::thread::hardware_concurrency();

        std::vector<std::jthread> threads;
        threads.reserve(num_threads);

        {
            for (int i = 0; i < num_threads; i++)
            {
                threads.emplace_back(worker);
            }
        }

        auto results = store.snapshot();
        print_table(results);

        return 0;
    }

} 