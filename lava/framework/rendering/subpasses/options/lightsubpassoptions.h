//
// Created by oshikuru on 20. 01. 2020..
//

#ifndef DEFERRED_LIGHTSUBPASSOPTIONS_H
#define DEFERRED_LIGHTSUBPASSOPTIONS_H

#include "lava/framework/object.h"
#include "lava/framework/rendering/subpasses/lightrenderer.h"

namespace LavaVk
{
    class LightSubpassOptions : public virtual Object
    {
    public:
        LightSubpassOptions() = default;
        explicit LightSubpassOptions(SharedLightRenderer renderer);

        SharedLightRenderer renderer;

        std::type_index getType() const override;
    };
}


#endif //DEFERRED_LIGHTSUBPASSOPTIONS_H
