#ifndef   _PURRFECT_ENGINE_WINDOW_HPP_
#define   _PURRFECT_ENGINE_WINDOW_HPP_

namespace PurrfectEngine {

  class Window {
  public:
    Window(const char *title, int32_t width, int32_t height);
    ~Window();

    bool shouldClose() const;

    bool isValid() const { return mValid; }

    inline purrr_window_t *getWindow() const { return mWindow; }
  private:
    purrr_window_t *mWindow = nullptr;
    bool mValid = false;
  };

}

#endif // _PURRFECT_ENGINE_WINDOW_HPP_