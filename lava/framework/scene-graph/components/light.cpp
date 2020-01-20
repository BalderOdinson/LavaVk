//
// Created by dorian on 14. 12. 2019..
//

#include "light.h"

#include <utility>

LavaVk::Light::Light(std::string_view name) : Component(name)
{
}

std::type_index LavaVk::Light::getType() const
{
    return typeid(Light);
}

LavaVk::SharedNode LavaVk::Light::getNode()
{
    return node.expired() ? nullptr : node.lock();
}

void LavaVk::Light::onAddedToNode(const LavaVk::SharedNode &n)
{
    Component::onAddedToNode(n);
    node = n;
}

void LavaVk::Light::onRemovedFormNode(const LavaVk::SharedNode &n)
{
    Component::onRemovedFormNode(n);
    node = {};
}
