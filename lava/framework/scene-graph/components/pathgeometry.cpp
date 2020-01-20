//
// Created by oshikuru on 11. 01. 2020..
//

#include "pathgeometry.h"

LavaVk::PathGeometry::PathGeometry(const LavaVk::SharedVertexSource &source) : Geometry(source)
{}

std::type_index LavaVk::PathGeometry::getType() const
{
    return typeid(PathGeometry);
}

std::vector<LavaVk::SharedNode> LavaVk::PathGeometry::getNodes() const
{
    std::vector<SharedNode> result(nodes.size());
    for (size_t i = 0; i < nodes.size(); ++i)
        result[i] = nodes[i].expired() ? nullptr : nodes[i].lock();
    return result;
}

void LavaVk::PathGeometry::onAddedToNode(const LavaVk::SharedNode &node)
{
    Component::onAddedToNode(node);
    nodes.emplace_back(node);
}

void LavaVk::PathGeometry::onRemovedFormNode(const LavaVk::SharedNode &node)
{
    Component::onRemovedFormNode(node);
    auto iter = std::find_if(nodes.begin(), nodes.end(),
                             [&node](const WeakNode& n)
                             {
                                 return !n.expired() && n.lock() == node;
                             });
    if(iter != nodes.end())
        nodes.erase(iter);
}
