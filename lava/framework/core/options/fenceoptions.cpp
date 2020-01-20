//
// Created by dorian on 09. 12. 2019..
//

#include "fenceoptions.h"

LavaVk::Core::FenceOptions::FenceOptions(FenceState signaled) : signaled(signaled)
{

}

std::type_index LavaVk::Core::FenceOptions::getType() const
{
    return typeid(FenceOptions);
}
