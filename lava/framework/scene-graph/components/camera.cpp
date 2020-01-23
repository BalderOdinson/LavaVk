//
// Created by dorian on 14. 12. 2019..
//

#include "camera.h"

#include <stdexcept>
#include <utility>

LavaVk::Camera::Camera(std::string_view name) : Component(name)
{
}

std::type_index LavaVk::Camera::getType() const
{
    return typeid(Camera);
}

glm::mat4 LavaVk::Camera::getView()
{
    if (node.expired())
        throw std::runtime_error{"Camera component is not attached to a node"};

    auto transform = node.lock()->getComponent<Transform>();
    return preRotation * glm::inverse(transform->getWorldMatrix());
}

LavaVk::SharedNode LavaVk::Camera::getNode() const
{
    return node.expired() ? nullptr : node.lock();
}

void LavaVk::Camera::setPreRotation(const glm::mat4 &newPreRotation)
{
    preRotation = newPreRotation;
}

void LavaVk::Camera::onAddedToNode(const LavaVk::SharedNode &n)
{
    Component::onAddedToNode(n);
    node = n;
}

void LavaVk::Camera::onRemovedFormNode(const LavaVk::SharedNode &n)
{
    Component::onRemovedFormNode(n);
    node = {};
}
