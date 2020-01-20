//
// Created by dorian on 15. 12. 2019..
//

#ifndef LAVAVK_PERSPECTIVECAMERA_H
#define LAVAVK_PERSPECTIVECAMERA_H

#include "camera.h"

namespace LavaVk
{
    class PerspectiveCamera : public Camera
    {
    public:
        explicit PerspectiveCamera(std::string_view name);

        void setAspectRatio(float aspectRatio);
        void setFieldOfView(float fov);
        void setFarPlane(float zfar);
        void setNearPlane(float znear);
        float getAspectRatio();
        float getFieldOfView();
        glm::mat4 getProjection() override;

    private:
        /// \brief Screen size aspect ratio
        float aspectRatio{1.0f};
        /// \brief Horizontal field of view in radians
        float fov{glm::radians(60.0f)};
        float farPlane{100.0};
        float nearPlane{0.1f};
    };
}

#endif //LAVAVK_PERSPECTIVECAMERA_H
