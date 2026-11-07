#pragma once

#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>

#include "cli/options.h"

namespace filescope::cli 
{
    class Runner 
    {
    private:
        std::atomic<size_t> next_file_id{ 0 };

    public:
        int run(const Options& opts);
    };
}