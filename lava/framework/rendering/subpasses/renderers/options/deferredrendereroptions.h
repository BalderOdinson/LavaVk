//
// Created by oshikuru on 20. 01. 2020..
//

#ifndef DEFERRED_DEFERREDRENDEREROPTIONS_H
#define DEFERRED_DEFERREDRENDEREROPTIONS_H

#include "lava/framework/object.h"
#include "lava/framework/rendering/subpasses/componentrenderer.h"

namespace LavaVk
{
    class DeferredRendererOptions : public virtual Object
    {
    public:
        DeferredRendererOptions() = default;
        explicit DeferredRendererOptions(SharedComponentRenderer baseRenderer, std::string_view shaderNameExtension = "df");

        SharedComponentRenderer baseRenderer;
        std::string shaderNameExtension{"df"};

        std::type_index getType() const override;
    };
}


#endif //DEFERRED_DEFERREDRENDEREROPTIONS_H
