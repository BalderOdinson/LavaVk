//
// Created by dorian on 07. 12. 2019..
//

#include "inputmanager.h"

LavaVk::Event<LavaVk::InputEventHandler> &LavaVk::InputManager::event()
{
    return eventOwner;
}

std::type_index LavaVk::InputManager::getType() const
{
    return typeid(InputManager);
}
