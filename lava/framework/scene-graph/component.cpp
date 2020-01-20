//
// Created by dorian on 08. 12. 2019..
//

#include "component.h"
#include "lava/framework/platform/application.h"

LavaVk::Component::Component(std::string_view name) : name(name)
{
}

const std::string &LavaVk::Component::getName() const
{
    return name;
}

void LavaVk::Component::setName(std::string_view n)
{
    name = n;
}

void LavaVk::Component::onAddedToNode(const LavaVk::SharedNode &node)
{
}

void LavaVk::Component::onRemovedFormNode(const LavaVk::SharedNode &node)
{
}

void LavaVk::Component::onAddedToScene(const LavaVk::SharedScene &node)
{
}

void LavaVk::Component::onRemovedFormScene(const LavaVk::SharedScene &node)
{
}
