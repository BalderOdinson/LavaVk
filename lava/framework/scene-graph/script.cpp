//
// Created by dorian on 08. 12. 2019..
//

#include "script.h"

#include <utility>

LavaVk::Script::Script(std::string_view name) : Component(name)
{
}

LavaVk::SharedNode LavaVk::Script::getNode()
{
    return node.expired() ? nullptr : node.lock();
}

std::type_index LavaVk::Script::getType() const
{
    return typeid(Script);
}

void LavaVk::Script::onAddedToNode(const LavaVk::SharedNode &n)
{
    Component::onAddedToNode(n);
    node = n;
}

void LavaVk::Script::onRemovedFormNode(const LavaVk::SharedNode &n)
{
    Component::onRemovedFormNode(n);
    node = {};
}
