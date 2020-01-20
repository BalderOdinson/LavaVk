//
// Created by dorian on 07. 12. 2019..
//

#ifndef LAVAVK_WINDOW_H
#define LAVAVK_WINDOW_H

#include <vulkan/vulkan.hpp>
#include <memory>
#include "lava/framework/object.h"
#include "inputmanager.h"

namespace LavaVk
{
    /// Arguments for resize event.
    class ResizeEventArgs
    {
    public:
        ResizeEventArgs(uint32_t width, uint32_t height, bool isFullscreen);

        uint32_t width;
        uint32_t height;
        bool isFullscreen;
    };

    /// Arguments for focus changed event.
    class FocusChangedEventArgs
    {
    public:
        explicit FocusChangedEventArgs(bool isFocused);

        bool isFocused;
    };

    /// An interface class, declaring the behaviour of a Window.
    class Window : public virtual Object
    {
    public:
        /// Gets extensions required for surface creation.
        /// \return extensions required for surface creation.
        virtual const char *getSurfaceExtensions() = 0;

        /// Gets a handle from the platform's Vulkan surface.
        /// \param instance The Vulkan instance.
        /// \return A VkSurfaceKHR handle, for use by the application.
        virtual vk::SurfaceKHR createSurface(const vk::Instance &instance) = 0;

        /// Checks if the window should be closed,
        /// \return if the window should be closed.
        virtual bool shouldClose() const = 0;

        /// Handles the processing of all underlying window events.
        virtual void processEvents() = 0;

        /// Requests to close the window.
        virtual void close() = 0;

        /// Gets the dot-per-inch scale factor.
        /// \return The dot-per-inch scale factor.
        virtual float getDpiFactor() const = 0;

        /// Resize window to given dimensions.
        virtual void resize(uint32_t width, uint32_t height) = 0;

        /// Gets if window is in full screen mode.
        /// \return if window is in full screen mode.
        virtual bool isFullscreen() = 0;

        /// Sets fullscreen mode.
        /// \param value is fullscreen mode.
        virtual void setFullscreen(bool value) = 0;

        /// Gets window width.
        /// \return window width.
        virtual uint32_t getWidth() const = 0;

        /// Gets window height.
        /// \return window height.
        virtual uint32_t getHeight() const = 0;

        /// Gets if window is focused.
        /// \return True if window is focused.
        virtual bool isFocused() const = 0;

        /// Gets resized event.
        /// \return resized event.
        Event<EventHandler<const ResizeEventArgs &>> &resized();

        /// Gets focus changed event.
        /// \return focus changed event.
        Event<EventHandler<const FocusChangedEventArgs &>> &focusChanged();

    protected:
        /// Gets input handler.
        /// \return input handler.
        virtual InputManager &input() = 0;

        /// Reports input event to manager.
        /// \param args input event arguments.
        void onInputReceived(const InputEventArgs &args);

        /// Focus changed event invocator.
        /// \param args
        void onFocusChanged(const FocusChangedEventArgs &args);

        /// Resized event invocator.
        /// \param args
        void onResized(const ResizeEventArgs &args);

    private:
        EventOwner<EventHandler<const ResizeEventArgs &>> resizeEventSubject;
        EventOwner<EventHandler<const FocusChangedEventArgs &>> focusChangedEventSubject;
    };

    using UniqueWindow = std::unique_ptr<Window>;
    using SharedWindow = std::shared_ptr<Window>;
    using WeakWindow = std::weak_ptr<Window>;
}


#endif //LAVAVK_WINDOW_H
