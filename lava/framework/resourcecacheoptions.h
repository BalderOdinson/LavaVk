//
// Created by oshikuru on 27. 12. 2019..
//

#ifndef LAVAVK_RESOURCECACHEOPTIONS_H
#define LAVAVK_RESOURCECACHEOPTIONS_H

#include "object.h"

namespace LavaVk
{
    class ResourceCacheOptions : public virtual Object
    {
    public:
        ResourceCacheOptions() = default;
        explicit ResourceCacheOptions(uint32_t cacheDuration);

        uint32_t cacheDuration{1};

        std::type_index getType() const override;
    };
}


#endif //LAVAVK_RESOURCECACHEOPTIONS_H
