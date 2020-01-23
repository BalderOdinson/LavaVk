//
// Created by dorian on 08. 12. 2019..
//

#include "transform.h"
#include <stdexcept>
#include "lava/framework/scene-graph/node.h"

LavaVk::Transform::Transform(const LavaVk::SharedNode &node) : node(node)
{
}

std::type_index LavaVk::Transform::getType() const
{
    return typeid(Transform);
}


LavaVk::SharedNode LavaVk::Transform::getNode()
{
    return node.expired() ? nullptr : node.lock();
}

void LavaVk::Transform::setTranslation(const glm::vec3 &newTranslation)
{
    translation = newTranslation;
    invalidateWorldMatrix();
}

void LavaVk::Transform::setRotation(const glm::quat &newRotation)
{
    rotation = newRotation;
    invalidateWorldMatrix();
}

void LavaVk::Transform::setScale(const glm::vec3 &newScale)
{
    scale = newScale;
    invalidateWorldMatrix();
}

const glm::vec3 &LavaVk::Transform::getTranslation() const
{
    return translation;
}

const glm::quat &LavaVk::Transform::getRotation() const
{
    return rotation;
}

const glm::vec3 &LavaVk::Transform::getScale() const
{
    return scale;
}

void LavaVk::Transform::setMatrix(const glm::mat4 &matrix)
{
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(matrix, scale, rotation, translation, skew, perspective);

    invalidateWorldMatrix();
}

glm::mat4 LavaVk::Transform::getMatrix() const
{
    return glm::translate(glm::mat4(1.0), translation) *
           glm::mat4_cast(rotation) *
           glm::scale(glm::mat4(1.0), scale);
}

glm::mat4 LavaVk::Transform::getWorldMatrix()
{
    updateWorldTransform();
    return worldMatrix;
}

void LavaVk::Transform::invalidateWorldMatrix()
{
    updateWorldMatrix = true;

    if(node.expired())
        throw std::runtime_error("Transform must be connected to Node!");

    auto children = node.lock()->getChildren();

    for (auto &child : children)
        child->getTransform()->invalidateWorldMatrix();
}

void LavaVk::Transform::updateWorldTransform()
{
    if (!updateWorldMatrix) return;

    worldMatrix = getMatrix();

    if(node.expired())
        throw std::runtime_error("Transform must be connected to Node!");

    auto parent = node.lock()->getParent();

    if (parent)
    {
        auto transform = parent->getComponent<Transform>();
        worldMatrix = transform->getWorldMatrix() * worldMatrix;
    }

    updateWorldMatrix = false;
}
