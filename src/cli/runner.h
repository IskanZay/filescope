#pragma once
#include "cli/options.h"

namespace filescope::cli 
{
    class Runner 
    {
    public:
        int run(const Options& opts);
    };
}