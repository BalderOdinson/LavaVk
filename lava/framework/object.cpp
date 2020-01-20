//
// Created by dorian on 06. 12. 2019..
//

#include "object.h"
#include <typeinfo>
#include <iostream>

bool LavaVk::Object::equals(const LavaVk::Object &other) const
{
    return this == &other;
}

std::string_view LavaVk::Object::toString() const
{
    return getType().name();
}

size_t LavaVk::Object::getHashCode() const
{
    return std::hash<size_t>()(reinterpret_cast<size_t>(this));
}
