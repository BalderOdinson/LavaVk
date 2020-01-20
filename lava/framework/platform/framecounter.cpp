//
// Created by dorian on 07. 12. 2019..
//

#include "framecounter.h"
#include "lava/framework/utils.h"

#include <utility>

LavaVk::FrameCounter::FrameCounter(LavaVk::SharedTimer timer, SharedLogger logger) : timer(std::move(timer)),
                                                                                     logger(std::move(logger))
{
}

LavaVk::FrameCounter::FrameCounter(LavaVk::InjectionContext &context) : timer(context.container.resolve<Timer>()),
                                                                        logger(context.container.resolve<Logger>())
{
}

void LavaVk::FrameCounter::nextFrame(float deltaTime)
{
    ++frameCount;
    auto elapsedTime = static_cast<float>(timer->elapsed<Timer::Seconds>());
    if (elapsedTime > 0.5f)
    {
        fps = frameCount / elapsedTime;
        frameTime = deltaTime * 1000.0f;
        frameCount = 0;
        timer->lap();
        logger->information(std::string_format("FPS: %f", fps));
    }
}

float LavaVk::FrameCounter::getFps() const
{
    return fps;
}

float LavaVk::FrameCounter::getFrameTime() const
{
    return frameTime;
}

std::type_index LavaVk::FrameCounter::getType() const
{
    return typeid(FrameCounter);
}
