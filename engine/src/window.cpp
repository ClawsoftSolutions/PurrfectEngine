#include "PurrfectEngine/PurrfectEngine.hpp"

namespace PurrfectEngine {

  Window::Window(const char *title, int32_t width, int32_t height)
  {
    purrr_window_info_t info = {
      PURRR_WINDOW_POS_CENTER, PURRR_WINDOW_POS_CENTER,
      width, height,
      title, false, PURRR_API_VULKAN,
      &mCallbacks,
    };
    if (!(mWindow = purrr_window_create(&info))) return;
    mValid = true;

    purrr_window_set_user_ptr(mWindow, &mQueues);

    mCallbacks->key = key_callback;
    mCallbacks->mouse_button = mouse_button_callback;
    mCallbacks->cursor_position = cursor_position_callback;
    mCallbacks->window_close = window_close_callback;
  }

  Window::~Window() {
    if (!mValid) return;
    purrr_window_destroy(mWindow);
  }

  bool Window::shouldClose() const {
    if (!mValid) return true;
    return purrr_window_should_close(mWindow);
  }

  void Window::subscribe(EventQueue *queue) {
    mQueues.push_back(queue);
  }



  void Window::key_callback(purrr_window_t* window, int key, int scancode, int action, int mods) {
    void *user_ptr = purrr_window_get_user_ptr(window);
    std::vector<EventQueue*> &queues = *(std::vector<EventQueue*>*)user_ptr;

    KeyPressEvent *event = new KeyPressEvent(key);
    for (auto &queue : queues) queue->enqueue(event);
  }

  void Window::mouse_button_callback(purrr_window_t* window, int button, int action, int mods) {
    void *user_ptr = purrr_window_get_user_ptr(window);
    std::vector<EventQueue*> &queues = *(std::vector<EventQueue*>*)user_ptr;

    MouseClickEvent *event = new MouseClickEvent(button);
    for (auto &queue : queues) queue->enqueue(event);
  }

  void Window::cursor_position_callback(purrr_window_t* window, double xpos, double ypos) {
    void *user_ptr = purrr_window_get_user_ptr(window);
    std::vector<EventQueue*> &queues = *(std::vector<EventQueue*>*)user_ptr;

    CursorMoveEvent *event = new CursorMoveEvent(xpos, ypos);
    for (auto &queue : queues) queue->enqueue(event);
  }

  bool Window::window_close_callback(purrr_window_t* window) {
    void *user_ptr = purrr_window_get_user_ptr(window);
    std::vector<EventQueue*> &queues = *(std::vector<EventQueue*>*)user_ptr;

    WindowCloseEvent *event = new WindowCloseEvent();
    for (auto &queue : queues) {
      queue->enqueue(event);
      if (event->isCancelled()) break;
    }
    return !event->isCancelled();
  }

}