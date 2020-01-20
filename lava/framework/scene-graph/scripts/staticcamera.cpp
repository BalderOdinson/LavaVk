//
// Created by oshikuru on 12. 01. 2020..
//

#include <lava/framework/scene-graph/components/camera.h>
#include <lava/framework/scene-graph/components/perspectivecamera.h>
#include "staticcamera.h"

LavaVk::StaticCamera::StaticCamera() : Script("StaticCamera")
{

}

LavaVk::StaticCamera::~StaticCamera()
{
    if(windowResized)
        window->resized().unsubscribe(windowResized);
}

void LavaVk::StaticCamera::initialize(LavaVk::DIContainer &container)
{
    window = container.resolve<Window>();

    windowResized = window->resized().subscribe(
            [&](Object& sender, const ResizeEventArgs &args)
            {
                auto cameraNode = getNode();

                if (cameraNode->hasComponent<Camera>())
                    if (auto camera = std::dynamic_pointer_cast<PerspectiveCamera>(cameraNode->getComponent<Camera>()))
                        camera->setAspectRatio(static_cast<float>(args.width) / args.height);
            });
}

void LavaVk::StaticCamera::update(float delta_time)
{

}
