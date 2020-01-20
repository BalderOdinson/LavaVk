//
// Created by dorian on 07. 12. 2019..
//

#ifndef LAVAVK_FRAMECOUNTER_H
#define LAVAVK_FRAMECOUNTER_H

#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "lava/framework/timer.h"
#include "lava/framework/object.h"
#include "logger.h"

namespace LavaVk
{
    /// Service for measuring performance.
    class FrameCounter : public virtual Object
    {
    public:
        FrameCounter(SharedTimer timer, SharedLogger logger);
        explicit FrameCounter(InjectionContext &context);

        /// Reports next frame;
        virtual void nextFrame(float deltaTime);

        /// Gets frame per second.
        /// \return frame count.
        virtual float getFps() const;

        /// Gets time(in seconds) per frame.
        /// \return time(in seconds) per frame.
        virtual float getFrameTime() const;

        std::type_index getType() const override;

    protected:
        SharedTimer timer;
        SharedLogger logger;

    private:
        uint32_t frameCount{0};
        float fps{0.f};
        float frameTime{0.f};
    };

    using UniqueFrameCounter = std::unique_ptr<FrameCounter>;
    using SharedFrameCounter = std::shared_ptr<FrameCounter>;
    using WeakFrameCounter = std::weak_ptr<FrameCounter>;
}


#endif //LAVAVK_FRAMECOUNTER_H
