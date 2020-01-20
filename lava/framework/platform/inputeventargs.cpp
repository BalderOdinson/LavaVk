//
// Created by dorian on 07. 12. 2019..
//

#include "inputeventargs.h"

LavaVk::EventSource LavaVk::KeyInputEventArgs::getSource() const
{
    return EventSource::Keyboard;
}

LavaVk::KeyInputEventArgs::KeyInputEventArgs(LavaVk::KeyCode code, LavaVk::KeyAction action) : code(code),
                                                                                               action(action)
{
}

LavaVk::KeyCode LavaVk::KeyInputEventArgs::getCode() const
{
    return code;
}

LavaVk::KeyAction LavaVk::KeyInputEventArgs::getAction() const
{
    return action;
}

LavaVk::EventSource LavaVk::MouseButtonInputEventArgs::getSource() const
{
    return EventSource::Mouse;
}

LavaVk::MouseButtonInputEventArgs::MouseButtonInputEventArgs(LavaVk::MouseButton button, LavaVk::MouseAction action,
                                                             float x, float y) : button(button), action(action), x(x),
                                                                                 y(y)
{
}

LavaVk::MouseButton LavaVk::MouseButtonInputEventArgs::getButton() const
{
    return button;
}

LavaVk::MouseAction LavaVk::MouseButtonInputEventArgs::getAction() const
{
    return action;
}

float LavaVk::MouseButtonInputEventArgs::getPositionX() const
{
    return x;
}

float LavaVk::MouseButtonInputEventArgs::getPositionY() const
{
    return y;
}

LavaVk::EventSource LavaVk::TouchInputEventArgs::getSource() const
{
    return EventSource::Touchscreen;
}

LavaVk::TouchInputEventArgs::TouchInputEventArgs(int32_t pointerId, size_t touchPoints, LavaVk::TouchAction action,
                                                 float x, float y) : action(action), pointerId(pointerId),
                                                                     touchPoints(touchPoints), x(x), y(y)
{
}

LavaVk::TouchAction LavaVk::TouchInputEventArgs::getAction() const
{
    return action;
}

int32_t LavaVk::TouchInputEventArgs::getPointerId() const
{
    return pointerId;
}

size_t LavaVk::TouchInputEventArgs::getTouchPoints() const
{
    return touchPoints;
}

float LavaVk::TouchInputEventArgs::getPositionX() const
{
    return x;
}

float LavaVk::TouchInputEventArgs::getPositionY() const
{
    return y;
}
