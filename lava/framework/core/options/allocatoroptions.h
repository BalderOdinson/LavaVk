//
// Created by dorian on 09. 12. 2019..
//

#ifndef LAVAVK_ALLOCATOROPTIONS_H
#define LAVAVK_ALLOCATOROPTIONS_H

#include "lava/framework/object.h"

namespace LavaVk::Core
{
    class AllocatorOptions : public virtual Object
    {
    public:
        explicit AllocatorOptions(uint32_t frameInUseCount);

        /// Sets how much objects that can get lost must stay alive (in frames).
        uint32_t frameInUseCount;

        std::type_index getType() const override;
    };
}


#endif //LAVAVK_ALLOCATOROPTIONS_H
