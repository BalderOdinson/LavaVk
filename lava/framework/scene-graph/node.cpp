//
// Created by dorian on 08. 12. 2019..
//

#include "node.h"
#include "components/transform.h"
#include "scene.h"

#include <utility>

LavaVk::Node::Node(std::string_view name) : name(name)
{
}

const std::string &LavaVk::Node::getName() const
{
    return name;
}

LavaVk::SharedTransform LavaVk::Node::getTransform()
{
    return transform;
}

void LavaVk::Node::setParent(const LavaVk::SharedNode &p)
{
    if (!parent.expired())
        parent.lock()->removeChild(std::dynamic_pointer_cast<Node>(shared_from_this()));

    parent = p;

    if (scene.expired() || scene.lock() != p->getScene())
        setScene(p->getScene());
}

LavaVk::SharedNode LavaVk::Node::getParent() const
{
    return parent.expired() ? nullptr : parent.lock();
}

void LavaVk::Node::addChild(const LavaVk::SharedNode &child)
{
    if (scene.expired())
        throw std::runtime_error("Node doesn't belong to valid scene!");
    child->setParent(std::dynamic_pointer_cast<Node>(shared_from_this()));
    children.insert(child);
}

const std::unordered_set<LavaVk::SharedNode> &LavaVk::Node::getChildren() const
{
    return children;
}

void LavaVk::Node::setComponent(const LavaVk::SharedComponent &component)
{
    SharedScene s{nullptr};
    if (!scene.expired())
        s = scene.lock();

    auto it = components.find(component->getType());

    if (it != components.end())
    {
        if (s) s->removeComponent(it->second);
        it->second = component;
        if (s) s->addComponent(it->second);
    }
    else
    {
        components.insert(std::make_pair(component->getType(), component));
        if (s) s->addComponent(component);
    }

    component->onAddedToNode(std::dynamic_pointer_cast<Node>(shared_from_this()));
}

LavaVk::SharedComponent LavaVk::Node::getComponent(const std::type_index &index)
{
    return components.at(index);
}

bool LavaVk::Node::hasComponent(const std::type_index &index)
{
    return components.count(index) > 0;
}

std::type_index LavaVk::Node::getType() const
{
    return typeid(Node);
}

LavaVk::SharedNode LavaVk::Node::make(std::string_view name)
{
    auto node = LavaVk::SharedNode(new Node(name));
    auto transform = LavaVk::make<Transform>(node);
    node->transform = transform;
    node->setComponent(transform);
    return node;
}

void LavaVk::Node::setScene(const LavaVk::SharedScene &s)
{
    if (!scene.expired())
    {
        auto lScene = scene.lock();
        for (auto &component : components)
            lScene->removeComponent(component.second);
    }
    scene = s;
    if (s)
    {
        s->addNode(std::dynamic_pointer_cast<Node>(shared_from_this()));
        for (auto &component : components)
            s->addComponent(component.second);
    }
}

LavaVk::SharedScene LavaVk::Node::getScene() const
{
    return scene.expired() ? nullptr : scene.lock();
}

void LavaVk::Node::removeChild(const LavaVk::SharedNode &child)
{
    children.erase(child);
}

void LavaVk::Node::removeComponent(const LavaVk::SharedComponent &component)
{
    auto componentIt = components.find(component->getType());
    if (componentIt != components.end() && componentIt->second == component)
    {
        components.erase(componentIt);
        component->onRemovedFormNode(std::dynamic_pointer_cast<Node>(shared_from_this()));
    }
}

size_t LavaVk::Node::getLayer() const
{
    return layer;
}

void LavaVk::Node::setLayer(size_t l)
{
    layer = l;
}
