//
// Created by dorian on 14. 12. 2019..
//

#ifndef LAVAVK_AABB_H
#define LAVAVK_AABB_H

#include "lava/third-party/glm.h"
#include "lava/framework/scene-graph/component.h"

namespace LavaVk
{
    class AABB;

    using UniqueAABB = std::unique_ptr<AABB>;
    using SharedAABB = std::shared_ptr<AABB>;
    using WeakAABB = std::weak_ptr<AABB>;

    class AABB : public Component
    {
    public:
        AABB();
        AABB(const glm::vec3 &min, const glm::vec3 &max);

        std::type_index getType() const override;

        /// \brief Update the bounding box based on the given vertex position
        /// \param point The 3D position of a point
        void update(const glm::vec3 &point);

        /// \brief Update the bounding box based on the given sub bounding box
        /// \param box Sub bounding box
        void update(const SharedAABB &box);

        /// \brief Update the bounding box based on the given submesh vertices
        /// \param vertexData The position vertex data
        /// \param indexData The index vertex data
        void update(const std::vector<glm::vec3> &vertexData, const std::vector<uint32_t> &indexData);

        /// \brief Apply a given matrix transformation to the bounding box
        /// \param transform The matrix transform to apply
        void transform(const glm::mat4 &transform);

        /// \brief Scale vector of the bounding box
        /// \return vector in 3D space
        glm::vec3 getScale() const;

        /// \brief Center position of the bounding box
        /// \return vector in 3D space
        glm::vec3 getCenter() const;

        /// \brief Minimum position of the bounding box
        /// \return vector in 3D space
        glm::vec3 getMin() const;

        /// \brief Maximum position of the bounding box
        /// \return vector in 3D space
        glm::vec3 getMax() const;

        /// \brief Resets the min and max position coordinates
        void reset();

    private:
        glm::vec3 min{};
        glm::vec3 max{};
    };
}


#endif //LAVAVK_AABB_H
