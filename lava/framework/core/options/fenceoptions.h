//
// Created by dorian on 09. 12. 2019..
//

#ifndef LAVAVK_FENCEOPTIONS_H
#define LAVAVK_FENCEOPTIONS_H

#include "lava/framework/object.h"

namespace LavaVk::Core
{
    enum class FenceState
    {
        Unsignaled,
        Signaled
    };

    class FenceOptions : public virtual Object
    {
    public:
        explicit FenceOptions(FenceState signaled);

        FenceState signaled;

        std::type_index getType() const override;
    };
}


#endif //LAVAVK_FENCEOPTIONS_H
