//
// Created by dorian on 09. 01. 2020..
//

#include "lava/framework/time.h"

void LavaVk::Time::update(float dt)
{
    delta = dt;
    total += dt;
}

void LavaVk::Time::reset()
{
    total = 0;
    delta = 0;
}

std::type_index LavaVk::Time::getType() const
{
    return typeid(Time);
}
