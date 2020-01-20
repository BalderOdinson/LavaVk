//
// Created by dorian on 15. 12. 2019..
//

#include "perspectivecamera.h"

#include <cmath>

LavaVk::PerspectiveCamera::PerspectiveCamera(std::string_view name) : Camera(name)
{
}

void LavaVk::PerspectiveCamera::setAspectRatio(float newAspectRatio)
{
    aspectRatio = newAspectRatio;
}

void LavaVk::PerspectiveCamera::setFieldOfView(float newFov)
{
    fov = newFov;
}

void LavaVk::PerspectiveCamera::setFarPlane(float zfar)
{
    farPlane = zfar;
}

void LavaVk::PerspectiveCamera::setNearPlane(float znear)
{
    nearPlane = znear;
}

float LavaVk::PerspectiveCamera::getAspectRatio()
{
    return aspectRatio;
}

float LavaVk::PerspectiveCamera::getFieldOfView()
{
    /* Calculate vertical fov */
    float vfov = 2 * atanf(tanf(fov / 2) * (1.0f / aspectRatio));

    return aspectRatio > 1.0f ? fov : vfov;
}

glm::mat4 LavaVk::PerspectiveCamera::getProjection()
{
    return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}
