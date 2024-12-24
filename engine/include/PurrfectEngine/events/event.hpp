#ifndef   _PURRFECT_ENGINE_EVENTS_EVENT_HPP_
#define   _PURRFECT_ENGINE_EVENTS_EVENT_HPP_

namespace PurrfectEngine {

  class Event {
  public:
    virtual ~Event() = default;

    virtual const char *getName() const = 0;

    inline void setHandled(bool handled) { mHandled = handled; }
    inline bool isHandled() const { return mHandled; }
  private:
    bool mHandled = false;
  };

  class KeyPressEvent : public Event {
  public:
    inline explicit KeyPressEvent(int key)
      : mKeyCode(key) {}

    inline const char *getName() const override { return "KeyPressEvent"; }
    inline int getKeyCode() const { return mKeyCode; }
  private:
    int mKeyCode;
  };

  class MouseClickEvent : public Event {
  public:
    inline explicit MouseClickEvent(int button)
      : mButton(button) {}

    const char *getName() const override { return "MouseClickEvent"; }
    int getButton() const { return mButton; }
  private:
    int mButton;
  };

  class WindowCloseEvent : public Event {
  public:
    const char *getName() const override { return "WindowCloseEvent"; }
  };

}

#endif // _PURRFECT_ENGINE_EVENTS_EVENT_HPP_