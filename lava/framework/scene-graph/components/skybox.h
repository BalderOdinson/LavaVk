//
// Created by oshikuru on 29. 12. 2019..
//

#ifndef LAVAVK_SKYBOX_H
#define LAVAVK_SKYBOX_H

#include <unordered_map>
#include "lava/framework/scene-graph/component.h"
#include "lava/framework/core/buffer.h"
#include "lava/framework/core/commandbuffer.h"
#include "lava/framework/meshes/meshsource.h"
#include "texture.h"
#include "lava/framework/materials/materialsource.h"
#include "mesh.h"

namespace LavaVk
{
    class Skybox : public Component
    {
    public:
        Skybox(SharedImageSource imageSource, SharedVertexSource vertexSource, std::string_view name = "");

        SharedGeometry getGeometry() const;
        std::unordered_map<std::string, SharedTexture> textures;
        SharedNode getNode() const;

        bool isValid() const;
        void load(const Core::SharedCommandBuffer &cmd);

        std::type_index getType() const override;

    protected:
        void onAddedToNode(const SharedNode &node) override;
        void onRemovedFormNode(const SharedNode &node) override;

    private:
        SharedGeometry geometry;
        WeakNode node;
    };

    using UniqueSkybox = std::unique_ptr<Skybox>;
    using SharedSkybox = std::shared_ptr<Skybox>;
    using WeakSkybox = std::weak_ptr<Skybox>;

}


#endif //LAVAVK_SKYBOX_H
