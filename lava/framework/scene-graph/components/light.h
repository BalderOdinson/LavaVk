//
// Created by dorian on 14. 12. 2019..
//

#ifndef LAVAVK_LIGHT_H
#define LAVAVK_LIGHT_H

#include "lava/framework/gl_includer.h"
#include "lava/framework/scene-graph/component.h"
#include "lava/framework/scene-graph/node.h"

namespace LavaVk
{
    enum LightType
    {
        Directional,
        Point,
        Spot
    };

    class Light : public Component
    {
    public:
        Light() = default;
        explicit Light(std::string_view name);
        Light(Light &&other) = default;

        std::type_index getType() const override;

        SharedNode getNode();
        LightType lightType{};
        glm::vec3 direction{0.0f, 0.0f, -1.0f};
        glm::vec3 color{1.0f, 1.0f, 1.0f};
        float intensityConstant{1.0f};
        float intensityLinear{0.0f};
        float intensityQuadratic{0.0f};
        float innerConeAngle{0.0f};
        float outerConeAngle{0.0f};

    protected:
        void onAddedToNode(const SharedNode &node) override;
        void onRemovedFormNode(const SharedNode &node) override;

    private:
        WeakNode node{};
    };

    using UniqueLight = std::unique_ptr<Light>;
    using SharedLight = std::shared_ptr<Light>;
    using WeakLight = std::weak_ptr<Light>;
}

#endif //LAVAVK_LIGHT_H
