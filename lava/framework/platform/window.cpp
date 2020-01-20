//
// Created by dorian on 07. 12. 2019..
//

#include "window.h"

void LavaVk::Window::onInputReceived(const LavaVk::InputEventArgs &args)
{
    input().eventOwner.raise<const LavaVk::InputEventArgs &>(*this, args);
}

void LavaVk::Window::onResized(const LavaVk::ResizeEventArgs &args)
{
    resizeEventSubject.raise(*this, args);
}

LavaVk::Event<LavaVk::EventHandler<const LavaVk::ResizeEventArgs &>> &LavaVk::Window::resized()
{
    return resizeEventSubject;
}

LavaVk::Event<LavaVk::EventHandler<const LavaVk::FocusChangedEventArgs &>> &LavaVk::Window::focusChanged()
{
    return focusChangedEventSubject;
}

void LavaVk::Window::onFocusChanged(const LavaVk::FocusChangedEventArgs &args)
{
    focusChangedEventSubject.raise(*this, args);
}

LavaVk::ResizeEventArgs::ResizeEventArgs(uint32_t width, uint32_t height, bool isFullscreen) : width(width),
                                                                                               height(height),
                                                                                               isFullscreen(
                                                                                                       isFullscreen)
{}

LavaVk::FocusChangedEventArgs::FocusChangedEventArgs(bool isFocused) : isFocused(isFocused)
{
}
