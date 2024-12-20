#ifndef _PURRFECT_ENGINE_EVENTS_EVENT_HPP_
#define _PURRFECT_ENGINE_EVENTS_EVENT_HPP_

namespace PurrfectEngine {
    enum class EventType {
        WindowClose,
        KeyPress,
        MouseClick,
    };

    class Event {
        bool handled = false;

    public:
        virtual ~Event() = default;
        virtual EventType getType() const = 0;
        virtual const char* getName() const = 0;

        void setHandled(bool isHandled) { handled = isHandled; }
        bool isHandled() const { return handled; }
    };

    class KeyPressEvent : public Event {
        int keyCode;

    public:
        explicit KeyPressEvent(int key) : keyCode(key) {}
        EventType getType() const override { return EventType::KeyPress; }
        const char* getName() const override { return "KeyPressEvent"; }
        int getKeyCode() const { return keyCode; }
    };

    class MouseClickEvent : public Event {
        int button;

    public:
        explicit MouseClickEvent(int button) : button(button) {}
        EventType getType() const override { return EventType::MouseClick; }
        const char* getName() const override { return "MouseClickEvent"; }
        int getButton() const { return button; }
    };

    class WindowCloseEvent : public Event {
    public:
        EventType getType() const override { return EventType::WindowClose; }
        const char* getName() const override { return "WindowCloseEvent"; }
    };
}

#endif // !_PURRFECT_ENGINE_EVENTS_EVENT_HPP_
