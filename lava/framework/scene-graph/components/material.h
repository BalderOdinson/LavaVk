//
// Created by dorian on 14. 12. 2019..
//

#ifndef LAVAVK_MATERIAL_H
#define LAVAVK_MATERIAL_H

#include <unordered_map>
#include "lava/framework/scene-graph/component.h"
#include "lava/framework/gl_includer.h"
#include "texture.h"

namespace LavaVk
{
    /// \brief How the alpha value of the main factor and texture should be interpreted
    enum class AlphaMode
    {
        /// Alpha value is ignored
                Opaque,
        /// Either full opaque or fully transparent
                Mask,
        /// Output is combined with the background
                Blend
    };

    class Material : public Component
    {
    public:
        explicit Material(std::string_view name);
        Material(Material &&other) = default;

        std::type_index getType() const override;

        std::unordered_map<std::string, SharedTexture> textures;

        /// Emissive color of the material
        glm::vec4 emissive{0.0f, 0.0f, 0.0f, 0.0f};
        /// Whether the material is double sided
        bool doubleSided{false};
        /// Cutoff threshold when in Mask mode
        float alphaCutoff{0.5f};
        /// Alpha rendering mode
        AlphaMode alphaMode{AlphaMode::Opaque};

        virtual std::vector<uint8_t> data() const = 0;

        virtual bool isValid();
        virtual void load(const Core::SharedCommandBuffer &commandBuffer);
    };

    using UniqueMaterial = std::unique_ptr<Material>;
    using SharedMaterial = std::shared_ptr<Material>;
    using WeakMaterial = std::weak_ptr<Material>;
}

#endif //LAVAVK_MATERIAL_H
