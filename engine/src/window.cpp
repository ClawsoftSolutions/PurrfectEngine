#include "PurrfectEngine/PurrfectEngine.hpp"

namespace PurrfectEngine {

  Window::Window(const char *title, int32_t width, int32_t height)
  {
    purrr_window_info_t info = {
      PURRR_WINDOW_POS_CENTER, PURRR_WINDOW_POS_CENTER,
      width, height,
      title, false, PURRR_API_VULKAN,
    };
    if (!(mWindow = purrr_window_create(&info))) return;
    mValid = true;
  }

  Window::~Window() {
    if (!mValid) return;
    purrr_window_destroy(mWindow);
  }

  bool Window::shouldClose() const {
    if (!mValid) return true;
    return purrr_window_should_close(mWindow);
  }

}