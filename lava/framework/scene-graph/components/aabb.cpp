//
// Created by dorian on 14. 12. 2019..
//

#include "aabb.h"

LavaVk::AABB::AABB()
{
    reset();
}

LavaVk::AABB::AABB(const glm::vec3 &min, const glm::vec3 &max) : min(min), max(max)
{
}

std::type_index LavaVk::AABB::getType() const
{
    return typeid(AABB);
}

void LavaVk::AABB::update(const glm::vec3 &point)
{
    min = glm::min(min, point);
    max = glm::max(max, point);
}

void LavaVk::AABB::update(const std::vector<glm::vec3> &vertexData, const std::vector<uint32_t> &indexData)
{
    // Check if submesh is indexed
    if (!indexData.empty())
        // Update bounding box for each indexed vertex
        for (unsigned short index_id : indexData)
            update(vertexData[index_id]);
    else
        // Update bounding box for each vertex
        for (auto vertex_id : vertexData)
            update(vertex_id);
}

void LavaVk::AABB::transform(const glm::mat4 &transform)
{
    auto oldMin = min;
    auto oldMax = max;

    reset();

    update(transform * glm::vec4(oldMin, 1.0f));

    // Update bounding box for the remaining 7 corners of the box
    update(transform * glm::vec4(oldMin.x, oldMin.y, oldMax.z, 1.0f));
    update(transform * glm::vec4(oldMin.x, oldMax.y, oldMin.z, 1.0f));
    update(transform * glm::vec4(oldMin.x, oldMax.y, oldMax.z, 1.0f));
    update(transform * glm::vec4(oldMax.x, oldMin.y, oldMin.z, 1.0f));
    update(transform * glm::vec4(oldMax.x, oldMin.y, oldMax.z, 1.0f));
    update(transform * glm::vec4(oldMax.x, oldMax.y, oldMin.z, 1.0f));
    update(transform * glm::vec4(oldMax, 1.0f));
}

glm::vec3 LavaVk::AABB::getScale() const
{
    return (max - min);
}

glm::vec3 LavaVk::AABB::getCenter() const
{
    return (min + max) * 0.5f;
}

glm::vec3 LavaVk::AABB::getMin() const
{
    return min;
}

glm::vec3 LavaVk::AABB::getMax() const
{
    return max;
}

void LavaVk::AABB::reset()
{
    min = glm::vec3(std::numeric_limits<float>::max());
    max = glm::vec3(std::numeric_limits<float>::min());
}

void LavaVk::AABB::update(const LavaVk::SharedAABB &box)
{
    min = glm::min(min, box->min);
    max = glm::max(max, box->max);
}
