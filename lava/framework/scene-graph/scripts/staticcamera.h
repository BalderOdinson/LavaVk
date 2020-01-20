//
// Created by oshikuru on 12. 01. 2020..
//

#ifndef LAVAVK_STATICCAMERA_H
#define LAVAVK_STATICCAMERA_H

#include "lava/framework/platform/window.h"
#include "lava/framework/scene-graph/script.h"

namespace LavaVk
{
    class StaticCamera : public Script
    {
    public:
        StaticCamera();
        ~StaticCamera() override;
        void initialize(DIContainer &container) override;
        void update(float delta_time) override;

    private:
        EventToken windowResized;
        SharedWindow window;
    };
}


#endif //LAVAVK_STATICCAMERA_H
