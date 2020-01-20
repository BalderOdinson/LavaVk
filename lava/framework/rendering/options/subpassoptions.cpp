//
// Created by dorian on 16. 12. 2019..
//

#include "subpassoptions.h"

#include <utility>

std::type_index LavaVk::SubpassOptions::getType() const
{
    return typeid(SubpassOptions);
}
