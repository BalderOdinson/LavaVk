//
// Created by dorian on 07. 12. 2019..
//

#ifndef LAVAVK_INPUTEVENTARGS_H
#define LAVAVK_INPUTEVENTARGS_H

#include <cstdint>
#include <cstddef>

namespace LavaVk
{
    enum class EventSource
    {
        Keyboard,
        Mouse,
        Touchscreen
    };

    class InputEventArgs
    {
    public:
        [[nodiscard]] virtual EventSource getSource() const = 0;
    };

    enum class KeyCode
    {
        Unknown,
        Space,
        Apostrophe, /* ' */
        Comma,      /* , */
        Minus,      /* - */
        Period,     /* . */
        Slash,      /* / */
        _0,
        _1,
        _2,
        _3,
        _4,
        _5,
        _6,
        _7,
        _8,
        _9,
        Semicolon, /* ; */
        Equal,     /* = */
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        LeftBracket,  /* [ */
        Backslash,    /* \ */
        RightBracket, /* ] */
        GraveAccent,  /* ` */
        Escape,
        Enter,
        Tab,
        Backspace,
        Insert,
        DelKey,
        Right,
        Left,
        Down,
        Up,
        PageUp,
        PageDown,
        Home,
        End,
        Back,
        CapsLock,
        ScrollLock,
        NumLock,
        PrintScreen,
        Pause,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        KP_0,
        KP_1,
        KP_2,
        KP_3,
        KP_4,
        KP_5,
        KP_6,
        KP_7,
        KP_8,
        KP_9,
        KP_Decimal,
        KP_Divide,
        KP_Multiply,
        KP_Subtract,
        KP_Add,
        KP_Enter,
        KP_Equal,
        LeftShift,
        LeftControl,
        LeftAlt,
        RightShift,
        RightControl,
        RightAlt
    };

    enum class KeyAction
    {
        Down,
        Up,
        Repeat,
        Unknown
    };

    class KeyInputEventArgs : public InputEventArgs
    {
    public:
        KeyInputEventArgs(KeyCode code, KeyAction action);

        [[nodiscard]] KeyCode getCode() const;
        [[nodiscard]] KeyAction getAction() const;

        EventSource getSource() const override;

    private:
        KeyCode code;
        KeyAction action;
    };

    enum class MouseButton
    {
        Left,
        Right,
        Middle,
        Back,
        Forward,
        Unknown
    };

    enum class MouseAction
    {
        Down,
        Up,
        Move,
        Unknown
    };

    class MouseButtonInputEventArgs : public InputEventArgs
    {
    public:
        MouseButtonInputEventArgs(MouseButton button, MouseAction action, float x, float y);
        EventSource getSource() const override;

        [[nodiscard]] MouseButton getButton() const;
        [[nodiscard]] MouseAction getAction() const;
        [[nodiscard]] float getPositionX() const;
        [[nodiscard]] float getPositionY() const;

    private:
        MouseButton button;
        MouseAction action;
        float x;
        float y;
    };

    enum class TouchAction
    {
        Down,
        Up,
        Move,
        Cancel,
        PointerDown,
        PointerUp,
        Unknown
    };

    class TouchInputEventArgs : public InputEventArgs
    {
    public:
        TouchInputEventArgs(int32_t pointerId, size_t touchPoints, TouchAction action, float x, float y);

        TouchAction getAction() const;
        int32_t getPointerId() const;
        size_t getTouchPoints() const;
        float getPositionX() const;
        float getPositionY() const;
        EventSource getSource() const override;

    private:
        TouchAction action;
        int32_t pointerId;
        size_t touchPoints;
        float x;
        float y;
    };
}


#endif //LAVAVK_INPUTEVENTARGS_H
