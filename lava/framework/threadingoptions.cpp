//
// Created by oshikuru on 06. 02. 2020..
//

#include "threadingoptions.h"
#include <omp.h>

LavaVk::ThreadingOptions::ThreadingOptions()
{
    setThreadCount(1);
}

LavaVk::ThreadingOptions::ThreadingOptions(uint32_t threadCount)
{
    setThreadCount(threadCount);
}

std::type_index LavaVk::ThreadingOptions::getType() const
{
    return typeid(ThreadingOptions);
}

uint32_t LavaVk::ThreadingOptions::getThreadCount() const
{
    return omp_get_max_threads();
}

void LavaVk::ThreadingOptions::setThreadCount(uint32_t threadCount)
{
    omp_set_num_threads(threadCount);
}
