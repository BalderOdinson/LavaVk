//
// Created by dorian on 08. 12. 2019..
//

#ifndef LAVAVK_TRANSFORM_H
#define LAVAVK_TRANSFORM_H

#include "lava/framework/scene-graph/component.h"
#include "lava/framework/gl_includer.h"

namespace LavaVk
{
    class Node;

    class Node;

    using UniqueNode = std::unique_ptr<Node>;
    using SharedNode = std::shared_ptr<Node>;
    using WeakNode = std::weak_ptr<Node>;

    class Transform : public Component
    {
    public:
        explicit Transform(const SharedNode &node);
        SharedNode getNode();
        void setTranslation(const glm::vec3 &translation);
        void setRotation(const glm::quat &rotation);
        void setScale(const glm::vec3 &scale);
        const glm::vec3 &getTranslation() const;
        const glm::quat &getRotation() const;
        const glm::vec3 &getScale() const;
        void setMatrix(const glm::mat4 &matrix);
        glm::mat4 getMatrix() const;
        glm::mat4 getWorldMatrix();
        /// \brief Marks the world transform invalid if any of
        /// the local transform are changed or the parent
        /// world transform has changed.
        void invalidateWorldMatrix();
        std::type_index getType() const override;

    private:
        WeakNode node;
        glm::vec3 translation{0.0, 0.0, 0.0};
        glm::quat rotation{1.0, 0.0, 0.0, 0.0};
        glm::vec3 scale{1.0, 1.0, 1.0};
        glm::mat4 worldMatrix{glm::mat4(1.0)};
        bool updateWorldMatrix{false};

        void updateWorldTransform();
    };

    using UniqueTransform = std::unique_ptr<Transform>;
    using SharedTransform = std::shared_ptr<Transform>;
    using WeakTransform = std::weak_ptr<Transform>;
}

#endif //LAVAVK_TRANSFORM_H
