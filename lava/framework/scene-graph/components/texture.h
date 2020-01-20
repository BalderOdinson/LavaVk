//
// Created by dorian on 14. 12. 2019..
//

#ifndef LAVAVK_TEXTURE_H
#define LAVAVK_TEXTURE_H

#include "lava/framework/scene-graph/component.h"
#include "image.h"
#include "sampler.h"

namespace LavaVk
{
    class Texture : public Component
    {
    public:
        explicit Texture(std::string_view name = "");
        Texture(SharedImage image, SharedSampler sampler, std::string_view name = "");
        Texture(Texture &&other) = default;

        bool isValid();
        void load(const Core::SharedCommandBuffer &commandBuffer);

        std::type_index getType() const override;

        SharedImage getImage() const;
        SharedSampler getSampler() const;

    private:
        SharedImage image{};
        SharedSampler sampler{};
    };

    using UniqueTexture = std::unique_ptr<Texture>;
    using SharedTexture = std::shared_ptr<Texture>;
    using WeakTexture = std::weak_ptr<Texture>;
}


#endif //LAVAVK_TEXTURE_H
