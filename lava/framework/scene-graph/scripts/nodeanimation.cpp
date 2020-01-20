//
// Created by dorian on 16. 12. 2019..
//

#include "nodeanimation.h"

#include <utility>

LavaVk::NodeAnimation::NodeAnimation(LavaVk::TransformAnimFn animationFn) : Script("Animation"),
                                                                            animationFn(
                                                                                    std::move(animationFn))
{
}

void LavaVk::NodeAnimation::initialize(LavaVk::DIContainer &container)
{
}

void LavaVk::NodeAnimation::update(float deltaTime)
{
    if (animationFn)
        animationFn(getNode()->getComponent<Transform>(), deltaTime);
}

void LavaVk::NodeAnimation::setAnimation(LavaVk::TransformAnimFn handle)
{
    animationFn = std::move(handle);
}

void LavaVk::NodeAnimation::clearAnimation()
{
    animationFn = {};
}
