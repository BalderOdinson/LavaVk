//
// Created by dorian on 31. 12. 2019..
//

#ifndef LAVAVK_MEMORYVERTEXSOURCE_H
#define LAVAVK_MEMORYVERTEXSOURCE_H

#include "vertexsource.h"

namespace LavaVk
{
    class MemoryVertexSource : public VertexSource
    {
    public:
        MemoryVertexSource(std::vector<uint8_t> &&vertices, size_t vertexCount, SharedVertexOptions options,
                           const std::vector<uint16_t> &indices, SharedAABB bounds);
        MemoryVertexSource(std::vector<uint8_t> &&vertices, size_t vertexCount, SharedVertexOptions options,
                           const std::vector<uint32_t> &indices, SharedAABB bounds);

        std::tuple<size_t, size_t, SharedVertexOptions, SharedAABB> load(std::vector<uint8_t> &vertices, std::vector<uint8_t> &indices) override;
        std::type_index getType() const override;

    private:
        std::vector<uint8_t> verticesData;
        size_t vertexCount;
        std::vector<uint8_t> indicesData;
        size_t indexCount;
        SharedVertexOptions options;
        SharedAABB bounds;
    };
}


#endif //LAVAVK_MEMORYVERTEXSOURCE_H
