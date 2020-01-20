//
// Created by dorian on 15. 12. 2019..
//

#ifndef LAVAVK_FREECAMERA_H
#define LAVAVK_FREECAMERA_H

#include <unordered_map>
#include "lava/third-party/glm.h"
#include "lava/framework/platform/window.h"
#include "lava/framework/platform/inputmanager.h"
#include "lava/framework/scene-graph/script.h"
#include "lava/framework/platform/inputeventargs.h"
#include "lava/framework/event-system/event.h"

namespace LavaVk
{
    class FreeCamera : public Script
    {
    public:
        FreeCamera();
        ~FreeCamera() override;
        void initialize(DIContainer &container) override;
        void update(float delta_time) override;

        float touchDownMoveForwardWaitTime{2.0f};
        float rotationMoveWeight{0.1f};
        float keyRotationMoveWeight{0.5f};
        float translationMoveWeight{3.0f};
        float translationMoveStep{50.0f};
        uint32_t translationMoveSpeed{4};

    private:
        float speedMultiplier{3.0f};
        glm::vec2 mouseMoveDelta{0.0f};
        glm::vec2 mouseLastPos{0.0f};
        glm::vec2 touchMoveDelta{0.0f};
        glm::vec2 touchLastPos{0.0f};
        float touchPointerTime{0.0f};
        std::unordered_map<KeyCode, bool> keyPressed;
        std::unordered_map<MouseButton, bool> mouseButtonPressed;
        std::unordered_map<int32_t, bool> touchPointerPressed;
        EventToken windowResized;
        EventToken inputReceived;
        SharedInputManager inputManager;
        SharedWindow window;
    };
}

#endif //LAVAVK_FREECAMERA_H
