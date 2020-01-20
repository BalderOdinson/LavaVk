//
// Created by dorian on 07. 12. 2019..
//

#ifndef LAVAVK_INPUTMANAGER_H
#define LAVAVK_INPUTMANAGER_H

#include "lava/framework/object.h"
#include "lava/framework/event-system/event.h"
#include "inputeventargs.h"
#include "lava/framework/utils.h"

namespace LavaVk
{
    using InputEventHandler = EventHandler<const InputEventArgs&>;

    /// Class for handling user input.
    class InputManager : public virtual Object
    {
    public:
        friend class Window;

        /// Gets input event.
        /// \return input event.
        Event<InputEventHandler> &event();

        std::type_index getType() const override;
    protected:
        EventOwner<InputEventHandler> eventOwner;
    };

    using UniqueInputManager = std::unique_ptr<InputManager>;
    using SharedInputManager = std::shared_ptr<InputManager>;
    using WeakInputManager = std::weak_ptr<InputManager>;
}


#endif //LAVAVK_INPUTMANAGER_H
