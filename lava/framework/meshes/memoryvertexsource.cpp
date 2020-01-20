//
// Created by dorian on 31. 12. 2019..
//

#include "memoryvertexsource.h"

LavaVk::MemoryVertexSource::MemoryVertexSource(std::vector<uint8_t> &&vertices, size_t vertexCount,
                                               LavaVk::SharedVertexOptions options,
                                               const std::vector<uint16_t> &indices, SharedAABB bounds)
        : verticesData(std::move(vertices)), vertexCount(vertexCount), indexCount(indices.size()),
          options(std::move(options)), bounds(std::move(bounds))
{
    indicesData = std::vector<uint8_t>(reinterpret_cast<const uint8_t *>(indices.data()),
                                       reinterpret_cast<const uint8_t *>(indices.data() + indices.size()));
}

LavaVk::MemoryVertexSource::MemoryVertexSource(std::vector<uint8_t> &&vertices, size_t vertexCount,
                                               LavaVk::SharedVertexOptions options,
                                               const std::vector<uint32_t> &indices, SharedAABB bounds)
        : verticesData(std::move(vertices)), vertexCount(vertexCount), indexCount(indices.size()),
          options(std::move(options)), bounds(std::move(bounds))
{
    indicesData = std::vector<uint8_t>(reinterpret_cast<const uint8_t *>(indices.data()),
                                       reinterpret_cast<const uint8_t *>(indices.data() + indices.size()));
}

std::tuple<size_t, size_t, LavaVk::SharedVertexOptions, LavaVk::SharedAABB>
LavaVk::MemoryVertexSource::load(std::vector<uint8_t> &vertices, std::vector<uint8_t> &indices)
{
    vertices = verticesData;
    indices = indicesData;
    return {vertexCount, indexCount, options, bounds};
}

std::type_index LavaVk::MemoryVertexSource::getType() const
{
    return typeid(MemoryVertexSource);
}
