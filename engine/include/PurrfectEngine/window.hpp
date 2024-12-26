#ifndef   _PURRFECT_ENGINE_WINDOW_HPP_
#define   _PURRFECT_ENGINE_WINDOW_HPP_

namespace PurrfectEngine {

  class EventQueue;
  class Window {
  public:
    Window(const char *title, int32_t width, int32_t height);
    ~Window();

    bool shouldClose() const;

    bool isValid() const { return mValid; }

    void subscribe(EventQueue *queue);

    inline purrr_window_t *getWindow() const { return mWindow; }
    inline purrr_window_callbacks_t *getCallbacks() const { return mCallbacks; }
  private:
    static void key_callback(purrr_window_t* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(purrr_window_t* window, int button, int action, int mods);
    static void cursor_position_callback(purrr_window_t* window, double xpos, double ypos);
    static bool window_close_callback(purrr_window_t* window);
  private:
    std::vector<EventQueue*> mQueues{};
  private:
    purrr_window_t *mWindow = nullptr;
    purrr_window_callbacks_t *mCallbacks = nullptr;
    bool mValid = false;
  };

}

#endif // _PURRFECT_ENGINE_WINDOW_HPP_