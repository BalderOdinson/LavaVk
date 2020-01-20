//
// Created by dorian on 10. 12. 2019..
//

#include "hostbufferupdater.h"
#include "lava/framework/utils.h"
#include "buffer.h"

void LavaVk::Core::HostBufferUpdater::update(LavaVk::Core::SharedBuffer buffer, void *data, size_t size, size_t offset,
                                             const vk::AccessFlags &currentAccess, const vk::AccessFlags &newAccess,
                                             const vk::PipelineStageFlags &generatingStages,
                                             const vk::PipelineStageFlags &consumingStages)
{

    auto allocInfo = buffer->getAllocationInfo();

    if (allocInfo.pMappedData)
    {
        buffer->invalidate(0, VK_WHOLE_SIZE);
        memcpy(offsetPointer(allocInfo.pMappedData, offset), data,
               std::min(size, allocInfo.size + offset));
        buffer->flush(0, VK_WHOLE_SIZE);
    }
    else
    {
        memcpy(offsetPointer(buffer->map(), offset), data,
               std::min(size, allocInfo.size + offset));
        buffer->unmap();
    }

}

std::type_index LavaVk::Core::HostBufferUpdater::getType() const
{
    return typeid(HostBufferUpdater);
}
