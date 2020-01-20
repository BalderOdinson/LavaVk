//
// Created by dorian on 09. 12. 2019..
//

#include "allocationoption.h"

LavaVk::Core::AllocationOption::AllocationOption(vma::MemoryUsage usage, bool isMapped, bool canGetLost,
                                                 const vk::MemoryPropertyFlags& requiredFlags,
                                                 const vk::MemoryPropertyFlags& preferredFlags) : memoryUsage(usage),
                                                                                           isMapped(isMapped),
                                                                                           canGetLost(canGetLost),
                                                                                           requiredFlags(requiredFlags),
                                                                                           preferredFlags(
                                                                                                   preferredFlags)
{
}
