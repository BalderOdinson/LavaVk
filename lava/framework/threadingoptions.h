//
// Created by oshikuru on 06. 02. 2020..
//

#ifndef DEFERRED_THREADINGOPTIONS_H
#define DEFERRED_THREADINGOPTIONS_H

#include "object.h"

namespace LavaVk
{
    class ThreadingOptions : public Object
    {
    public:
        ThreadingOptions();
        explicit ThreadingOptions(uint32_t threadCount);

        std::type_index getType() const override;

        uint32_t getThreadCount() const;
        void setThreadCount(uint32_t threadCount);
    };

    using UniqueThreadingOptions = std::unique_ptr<ThreadingOptions>;
    using SharedThreadingOptions = std::shared_ptr<ThreadingOptions>;
    using WeakThreadingOptions = std::weak_ptr<ThreadingOptions>;
}


#endif //DEFERRED_THREADINGOPTIONS_H
