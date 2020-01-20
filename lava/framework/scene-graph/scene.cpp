//
// Created by dorian on 08. 12. 2019..
//

#include "scene.h"

#include <utility>
#include <queue>

std::type_index LavaVk::Scene::getType() const
{
    return typeid(Scene);
}

LavaVk::Scene::Scene(std::string name) : name(std::move(name))
{
}

void LavaVk::Scene::setName(const std::string &newName)
{
    name = newName;
}

const std::string &LavaVk::Scene::getName() const
{
    return name;
}

void LavaVk::Scene::setNodes(std::unordered_set<SharedNode> &&n)
{
    nodes = std::move(n);
}

void LavaVk::Scene::addNode(LavaVk::SharedNode n)
{
    nodes.insert(std::move(n));
}

void LavaVk::Scene::addChild(LavaVk::SharedNode child)
{
    child->setScene(std::dynamic_pointer_cast<Scene>(shared_from_this()));
    children.insert(std::move(child));
}

const std::unordered_set<LavaVk::SharedNode> &LavaVk::Scene::getChildren() const
{
    return children;
}

void LavaVk::Scene::addComponent(const LavaVk::SharedComponent& component)
{
    if (component)
    {
        ++referenceCounts[component];
        components[component->getType()].insert(component);
        component->onAddedToScene(std::dynamic_pointer_cast<Scene>(shared_from_this()));
    }
}

void LavaVk::Scene::setComponents(const std::type_index &typeInfo, std::unordered_set<SharedComponent> &&newComponents)
{
    if(components.find(typeInfo) != components.end())
        for (auto &component : components[typeInfo])
        {
            referenceCounts.erase(component);
            component->onRemovedFormScene(std::dynamic_pointer_cast<Scene>(shared_from_this()));
        }

    components[typeInfo] = std::move(newComponents);

    for (auto &component : components[typeInfo])
    {
        ++referenceCounts[component];
        component->onAddedToScene(std::dynamic_pointer_cast<Scene>(shared_from_this()));
    }
}

const std::unordered_set<LavaVk::SharedComponent> &LavaVk::Scene::getComponents(const std::type_index &typeinfo) const
{
    return components.at(typeinfo);
}

bool LavaVk::Scene::hasComponent(const std::type_index &typeInfo) const
{
    return components.count(typeInfo) != 0;
}

LavaVk::SharedNode LavaVk::Scene::findNode(const std::string &nodeName)
{
    for (const auto &rootNode : children)
    {
        std::queue<SharedNode> traverse_nodes{};
        traverse_nodes.push(rootNode);

        while (!traverse_nodes.empty())
        {
            auto node = traverse_nodes.front();
            traverse_nodes.pop();

            if (node->getName() == nodeName)
                return node;

            for (const auto &childNode : node->getChildren())
                traverse_nodes.push(childNode);
        }
    }

    return nullptr;
}

void LavaVk::Scene::removeComponent(const LavaVk::SharedComponent &component)
{
    if(components.find(component->getType()) == components.end())
        return;
    --referenceCounts[component];
    if(referenceCounts[component] == 0)
    {
        referenceCounts.erase(component);
        components[component->getType()].erase(component);
        component->onRemovedFormScene(std::dynamic_pointer_cast<Scene>(shared_from_this()));
    }
}


