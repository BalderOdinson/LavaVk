//
// Created by dorian on 15. 12. 2019..
//

#include "freecamera.h"
#include "lava/framework/scene-graph/components/camera.h"
#include "lava/framework/scene-graph/components/perspectivecamera.h"


LavaVk::FreeCamera::FreeCamera() : Script("FreeCamera")
{
}

LavaVk::FreeCamera::~FreeCamera()
{
    if(windowResized)
        window->resized().unsubscribe(windowResized);
    if(inputReceived)
        inputManager->event().unsubscribe(inputReceived);
}

void LavaVk::FreeCamera::initialize(LavaVk::DIContainer &container)
{
    window = container.resolve<Window>();
    inputManager = container.resolve<InputManager>();

    windowResized = window->resized().subscribe(
            [&](Object& sender, const ResizeEventArgs &args)
            {
                auto cameraNode = getNode();

                if (cameraNode->hasComponent<Camera>())
                    if (auto camera = std::dynamic_pointer_cast<PerspectiveCamera>(cameraNode->getComponent<Camera>()))
                        camera->setAspectRatio(static_cast<float>(args.width) / args.height);
            });

    inputReceived = inputManager->event().subscribe(
            [&](Object& sender, const InputEventArgs& args)
            {
                if (args.getSource() == EventSource::Keyboard)
                {
                    const auto &keyEvent = dynamic_cast<const KeyInputEventArgs &>(args);

                    keyPressed[keyEvent.getCode()] = keyEvent.getAction() == KeyAction::Down ||
                                                     keyEvent.getAction() == KeyAction::Repeat;
                }
                else if (args.getSource() == EventSource::Mouse)
                {
                    const auto &mouseButton = dynamic_cast<const MouseButtonInputEventArgs &>(args);

                    glm::vec2 mousePos{std::floor(mouseButton.getPositionX()), std::floor(mouseButton.getPositionY())};

                    if (mouseButton.getAction() == MouseAction::Down)
                        mouseButtonPressed[mouseButton.getButton()] = true;

                    if (mouseButton.getAction() == MouseAction::Up)
                        mouseButtonPressed[mouseButton.getButton()] = false;

                    if (mouseButton.getAction() == MouseAction::Move)
                    {
                        mouseMoveDelta = mousePos - mouseLastPos;
                        mouseLastPos = mousePos;
                    }
                }
                else if (args.getSource() == EventSource::Touchscreen)
                {
                    const auto &touchEvent = dynamic_cast<const TouchInputEventArgs &>(args);

                    glm::vec2 touchPos{std::floor(touchEvent.getPositionX()), std::floor(touchEvent.getPositionY())};

                    if (touchEvent.getAction() == TouchAction::Down)
                    {
                        touchPointerPressed[touchEvent.getPointerId()] = true;
                        touchLastPos = touchPos;
                    }

                    if (touchEvent.getAction() == TouchAction::Up)
                    {
                        touchPointerPressed[touchEvent.getPointerId()] = false;
                        touchPointerTime = 0.0f;
                    }

                    if (touchEvent.getAction() == TouchAction::Move && touchEvent.getPointerId() == 0)
                    {
                        touchMoveDelta = touchPos - touchLastPos;
                        touchLastPos = touchPos;
                    }
                }
            });
}

void LavaVk::FreeCamera::update(float delta_time)
{
    glm::vec3 deltaTranslation(0.0f, 0.0f, 0.0f);
    glm::vec3 deltaRotation(0.0f, 0.0f, 0.0f);

    float mulTranslation = speedMultiplier;

    if (keyPressed[KeyCode::W])
        deltaTranslation.z -= translationMoveStep;
    if (keyPressed[KeyCode::S])
        deltaTranslation.z += translationMoveStep;
    if (keyPressed[KeyCode::A])
        deltaTranslation.x -= translationMoveStep;
    if (keyPressed[KeyCode::D])
        deltaTranslation.x += translationMoveStep;
    if (keyPressed[KeyCode::LeftControl])
        mulTranslation *= (1.0f * translationMoveSpeed);
    if (keyPressed[KeyCode::LeftShift])
        mulTranslation *= (1.0f / translationMoveSpeed);

    if (keyPressed[KeyCode::I])
        deltaRotation.x += keyRotationMoveWeight;
    if (keyPressed[KeyCode::K])
        deltaRotation.x -= keyRotationMoveWeight;
    if (keyPressed[KeyCode::J])
        deltaRotation.y += keyRotationMoveWeight;
    if (keyPressed[KeyCode::L])
        deltaRotation.y -= keyRotationMoveWeight;
    if (keyPressed[KeyCode::Q])
        deltaRotation.z = keyRotationMoveWeight;
    if (keyPressed[KeyCode::E])
        deltaRotation.z -= keyRotationMoveWeight;

    if (mouseButtonPressed[MouseButton::Left] && mouseButtonPressed[MouseButton::Right])
        deltaRotation.z += translationMoveWeight * mouseMoveDelta.x;
    else if (mouseButtonPressed[MouseButton::Right])
    {
        deltaRotation.x -= rotationMoveWeight * mouseMoveDelta.y;
        deltaRotation.y -= rotationMoveWeight * mouseMoveDelta.x;
    }
    else if (mouseButtonPressed[MouseButton::Left])
    {
        deltaTranslation.x += translationMoveWeight * mouseMoveDelta.x;
        deltaTranslation.y += translationMoveWeight * -mouseMoveDelta.y;
    }

    if (touchPointerPressed[0])
    {
        deltaRotation.x -= rotationMoveWeight * touchMoveDelta.y;
        deltaRotation.y -= rotationMoveWeight * touchMoveDelta.x;

        if (touchPointerTime > touchDownMoveForwardWaitTime)
            deltaTranslation.z -= translationMoveStep;
        else
            touchPointerTime += delta_time;
    }

    deltaTranslation *= mulTranslation * delta_time;
    deltaRotation *= delta_time;

    // Only re-calculate the transform if it's changed
    if (deltaRotation != glm::vec3(0.0f, 0.0f, 0.0f) || deltaTranslation != glm::vec3(0.0f, 0.0f, 0.0f))
    {
        auto transform = getNode()->getComponent<Transform>();

        glm::quat qx = glm::angleAxis(deltaRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat qy = glm::angleAxis(deltaRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat qz = glm::angleAxis(deltaRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

        glm::quat orientation = glm::normalize(transform->getRotation() * qz * qy * qx);

        transform->setTranslation(transform->getTranslation() + deltaTranslation * glm::conjugate(orientation));
        transform->setRotation(orientation);
    }

    mouseMoveDelta = {};
    touchMoveDelta = {};
}
