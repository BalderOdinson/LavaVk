//
// Created by dorian on 15. 12. 2019..
//

#ifndef LAVAVK_MESH_H
#define LAVAVK_MESH_H

#include <unordered_set>
#include "lava/framework/gl_includer.h"
#include "lava/framework/scene-graph/component.h"
#include "lava/framework/scene-graph/node.h"
#include "lava/framework/meshes/meshsource.h"
#include "aabb.h"
#include "submesh.h"

namespace LavaVk
{
    class Mesh : public Component
    {
    public:
        explicit Mesh(SharedMeshSource source, std::string_view name = "");

        std::type_index getType() const override;
        SharedAABB getBounds() const;
        void addSubMesh(SharedSubMesh subMesh);
        const std::vector<SharedSubMesh> &getSubmeshes() const;
        std::vector<SharedNode> getNodes() const;

        bool isValid();
        void load(const Core::SharedCommandBuffer &commandBuffer);

    protected:
        void onAddedToNode(const SharedNode &node) override;
        void onRemovedFormNode(const SharedNode &node) override;

    private:
        SharedAABB bounds;
        std::vector<SharedSubMesh> subMeshes;
        std::vector<WeakNode> nodes;
        SharedMeshSource source;
    };

    using UniqueMesh = std::unique_ptr<Mesh>;
    using SharedMesh = std::shared_ptr<Mesh>;
    using WeakMesh = std::weak_ptr<Mesh>;
}


#endif //LAVAVK_MESH_H
