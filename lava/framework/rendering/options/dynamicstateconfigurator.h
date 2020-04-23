//
// Created by oshikuru on 07. 02. 2020..
//

#ifndef DEFERRED_DYNAMICSTATECONFIGURATOR_H
#define DEFERRED_DYNAMICSTATECONFIGURATOR_H

#include "lava/framework/object.h"
#include "lava/framework/core/commandbuffer.h"

namespace LavaVk
{
    class DynamicStateConfigurator : public virtual Object
    {
    public:
        void setDynamicStates(const Core::SharedCommandBuffer &cmd);

        std::type_index getType() const override;

    protected:
        virtual void setViewportState(const Core::SharedCommandBuffer &cmd);
        virtual void setVertexInputState(const Core::SharedCommandBuffer &cmd);
        virtual void setInputAssemblyState(const Core::SharedCommandBuffer &cmd);
        virtual void setRasterizationState(const Core::SharedCommandBuffer &cmd);
        virtual void setMultisampleState(const Core::SharedCommandBuffer &cmd);
        virtual void setDepthStencilState(const Core::SharedCommandBuffer &cmd);
        virtual void setColorBlendState(const Core::SharedCommandBuffer &cmd);
        virtual void setViewport(const Core::SharedCommandBuffer &cmd);
        virtual void setScissor(const Core::SharedCommandBuffer &cmd);
        virtual void setLineWidth(const Core::SharedCommandBuffer &cmd);
        virtual void setDepthBias(const Core::SharedCommandBuffer &cmd);
        virtual void setBlendConstants(const Core::SharedCommandBuffer &cmd);
        virtual void setDepthBounds(const Core::SharedCommandBuffer &cmd);
    };

    using UniqueDynamicStateConfigurator = std::unique_ptr<DynamicStateConfigurator>;
    using SharedDynamicStateConfigurator = std::shared_ptr<DynamicStateConfigurator>;
    using WeakDynamicStateConfigurator = std::weak_ptr<DynamicStateConfigurator>;
}


#endif //DEFERRED_DYNAMICSTATECONFIGURATOR_H
