//
// Created by dorian on 14. 12. 2019..
//

#ifndef LAVAVK_CAMERA_H
#define LAVAVK_CAMERA_H

#include "lava/third-party/glm.h"
#include "lava/framework/scene-graph/component.h"
#include "lava/framework/scene-graph/node.h"

namespace LavaVk
{
    class Camera : public Component
    {
    public:
        explicit Camera(std::string_view name);

        std::type_index getType() const override;
        virtual glm::mat4 getProjection() = 0;
        glm::mat4 getView();
        SharedNode getNode() const;
        void setPreRotation(const glm::mat4 &preRotation);

    protected:
        void onAddedToNode(const SharedNode &node) override;
        void onRemovedFormNode(const SharedNode &node) override;

    private:
        WeakNode node{};
        glm::mat4 preRotation{1.0f};
    };

    using UniqueCamera = std::unique_ptr<Camera>;
    using SharedCamera = std::shared_ptr<Camera>;
    using WeakCamera = std::weak_ptr<Camera>;
}



#endif //LAVAVK_CAMERA_H
