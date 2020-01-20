//
// Created by dorian on 15. 12. 2019..
//

#include "mesh.h"

LavaVk::Mesh::Mesh(LavaVk::SharedMeshSource source, std::string_view name) : Component(name), bounds(make<AABB>()),
                                                                             source(std::move(source))
{
}

std::type_index LavaVk::Mesh::getType() const
{
    return typeid(Mesh);
}

LavaVk::SharedAABB LavaVk::Mesh::getBounds() const
{
    return bounds;
}

void LavaVk::Mesh::addSubMesh(LavaVk::SharedSubMesh subMesh)
{
    bounds->update(subMesh->getGeometry()->getBounds());
    subMeshes.emplace_back(std::move(subMesh));
}

const std::vector<LavaVk::SharedSubMesh> &LavaVk::Mesh::getSubmeshes() const
{
    return subMeshes;
}

std::vector<LavaVk::SharedNode> LavaVk::Mesh::getNodes() const
{
    std::vector<SharedNode> result(nodes.size());
    for (size_t i = 0; i < nodes.size(); ++i)
        result[i] = nodes[i].expired() ? nullptr : nodes[i].lock();
    return result;
}

bool LavaVk::Mesh::isValid()
{
    return !subMeshes.empty() && std::all_of(subMeshes.begin(), subMeshes.end(), [](const SharedSubMesh &s)
    { return s->isValid(); });
}

void LavaVk::Mesh::load(const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    for (auto &s : source->load(commandBuffer))
        addSubMesh(s);
}

void LavaVk::Mesh::onAddedToNode(const LavaVk::SharedNode &node)
{
    Component::onAddedToNode(node);
    nodes.emplace_back(node);
}

void LavaVk::Mesh::onRemovedFormNode(const LavaVk::SharedNode &node)
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
