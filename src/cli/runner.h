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
    public:
        int run(const Options& opts);
    };
}