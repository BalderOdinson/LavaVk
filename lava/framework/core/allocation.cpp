//
// Created by dorian on 09. 12. 2019..
//

#include "allocation.h"

bool LavaVk::Core::Allocation::touch() const
{
    return getAllocator()->getHandle().touchAllocation(getAllocation());
}

void LavaVk::Core::Allocation::invalidate(vk::DeviceSize offset, vk::DeviceSize size) const
{
    getAllocator()->getHandle().invalidateAllocation(getAllocation(), offset, size);
}

void LavaVk::Core::Allocation::flush(vk::DeviceSize offset, vk::DeviceSize size) const
{
    getAllocator()->getHandle().flushAllocation(getAllocation(), offset, size);
}

void *LavaVk::Core::Allocation::map() const
{
    return getAllocator()->getHandle().mapMemory(getAllocation());
}

void LavaVk::Core::Allocation::unmap() const
{
    getAllocator()->getHandle().unmapMemory(getAllocation());
}

vma::AllocationInfo LavaVk::Core::Allocation::getAllocationInfo() const
{
    return getAllocator()->getHandle().getAllocationInfo(getAllocation());
}
