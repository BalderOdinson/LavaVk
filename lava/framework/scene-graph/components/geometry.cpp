//
// Created by dorian on 07. 01. 2020..
//

#include "geometry.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/core/options/stagingbufferoptions.h"
#include "lava/framework/core/pipelinebarrier.h"

#include <utility>

LavaVk::Geometry::Geometry(LavaVk::SharedVertexSource source) : source(std::move(source))
{
}

std::type_index LavaVk::Geometry::getType() const
{
    return typeid(Geometry);
}

LavaVk::SharedVertexOptions LavaVk::Geometry::getVertexAttributes() const
{
    return attributes;
}

LavaVk::Core::SharedBuffer LavaVk::Geometry::getVertexBuffer(size_t binding) const
{
    return vertexBuffers[binding];
}

LavaVk::Core::SharedBuffer LavaVk::Geometry::getIndexBuffer()
{
    return indexBuffer;
}

size_t LavaVk::Geometry::getVertexCount() const
{
    return vertexCount;
}

size_t LavaVk::Geometry::getIndexCount() const
{
    return indexCount;
}

LavaVk::SharedAABB LavaVk::Geometry::getBounds() const
{
    return bounds;
}

bool LavaVk::Geometry::isValid()
{
    return !vertexBuffers.empty()
           && std::all_of(vertexBuffers.begin(), vertexBuffers.end(),
                          [](const Core::SharedBuffer &buffer)
                          { return buffer->touch(); })
           && (!indexBuffer || indexBuffer->touch());
}

void LavaVk::Geometry::load(const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    std::vector<uint8_t> verticesData;
    std::vector<uint8_t> indicesData;
    std::tie(vertexCount, indexCount, attributes, bounds) = source->load(verticesData, indicesData);
    vertexBuffers.clear();

    auto stagingBuffer = Application::instance->container.resolve<LavaVk::Core::Buffer, LavaVk::Core::StagingBufferOptions>(
            1, verticesData.size());
    stagingBuffer->update(verticesData.data(), verticesData.size(), 0, {}, {}, {}, {});
    commandBuffer->saveExecutionResource(stagingBuffer);

    vk::DeviceSize offset = 0;

    for (auto &vertex : attributes->getVertexInputs())
    {
        vertexBuffers.emplace_back(Application::instance->container.resolve<Core::Buffer, Core::BufferOptions>(
                vertex.stride, vertexCount,
                vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
                vma::MemoryUsage::eGpuOnly, false));


        commandBuffer->pipelineBarrier().addBufferMemoryBarrier(vertexBuffers.back()->getHandle(),
                                                                {},
                                                                vk::AccessFlagBits::eTransferWrite,
                                                                vk::PipelineStageFlagBits::eTopOfPipe,
                                                                vk::PipelineStageFlagBits::eTransfer,
                                                                VK_QUEUE_FAMILY_IGNORED,
                                                                VK_QUEUE_FAMILY_IGNORED,
                                                                offset, vertex.stride * vertexCount).execute();

        commandBuffer->copy(stagingBuffer, vertexBuffers.back()).addRegion(offset, 0,
                                                                           vertex.stride * vertexCount).execute();

        commandBuffer->pipelineBarrier().addBufferMemoryBarrier(vertexBuffers.back()->getHandle(),
                                                                vk::AccessFlagBits::eTransferWrite,
                                                                vk::AccessFlagBits::eVertexAttributeRead,
                                                                vk::PipelineStageFlagBits::eTransfer,
                                                                vk::PipelineStageFlagBits::eVertexInput,
                                                                VK_QUEUE_FAMILY_IGNORED,
                                                                VK_QUEUE_FAMILY_IGNORED,
                                                                offset, vertex.stride * vertexCount).execute();

        offset += vertex.stride * vertexCount;
    }

    if(indexCount > 0)
    {
        auto indexStagingBuffer = Application::instance->container.resolve<LavaVk::Core::Buffer, LavaVk::Core::StagingBufferOptions>(
                1, indicesData.size());
        indexStagingBuffer->update(indicesData.data(), indicesData.size(), 0, {}, {}, {}, {});
        commandBuffer->saveExecutionResource(indexStagingBuffer);

        indexBuffer = Application::instance->container.resolve<Core::Buffer, Core::BufferOptions>(
                1, indicesData.size(),
                vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
                vma::MemoryUsage::eGpuOnly, false);

        commandBuffer->pipelineBarrier().addBufferMemoryBarrier(indexBuffer->getHandle(),
                                                                {},
                                                                vk::AccessFlagBits::eTransferWrite,
                                                                vk::PipelineStageFlagBits::eTopOfPipe,
                                                                vk::PipelineStageFlagBits::eTransfer,
                                                                VK_QUEUE_FAMILY_IGNORED,
                                                                VK_QUEUE_FAMILY_IGNORED).execute();

        commandBuffer->copy(indexStagingBuffer, indexBuffer).addRegion(0, 0, indicesData.size()).execute();

        commandBuffer->pipelineBarrier().addBufferMemoryBarrier(indexBuffer->getHandle(),
                                                                vk::AccessFlagBits::eTransferWrite,
                                                                vk::AccessFlagBits::eIndexRead,
                                                                vk::PipelineStageFlagBits::eTransfer,
                                                                vk::PipelineStageFlagBits::eVertexInput,
                                                                VK_QUEUE_FAMILY_IGNORED,
                                                                VK_QUEUE_FAMILY_IGNORED).execute();
    }
}
