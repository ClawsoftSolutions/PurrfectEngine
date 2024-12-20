#ifndef EVENTS_CALLBACKS_HPP
#define EVENTS_CALLBACKS_HPP

#include "EventQueue.hpp"
#include "purrr/purrr.h"
#include <memory>
#include <GLFW/glfw3.h>
#include "PurrfectEngine/utils/WindowRendererMap.hpp"

namespace PurrfectEngine {
    class EventsCallbacks {
    private:
        EventQueue* eventQueue = nullptr;

    public:
        EventsCallbacks(EventQueue* queue) : eventQueue(queue) {};
        ~EventsCallbacks() {};

        static void keyCallback(purrr_window_t* window, int key, int scancode, int action, int mods) {
            auto* renderer = WindowRendererMap::get(window);
            if (!renderer) return;

            auto* eventQueue = static_cast<EventQueue*>(purrr_renderer_get_user_pointer(renderer));
            if (!eventQueue) return;

            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                eventQueue->enqueue(std::make_unique<KeyPressEvent>(key));
            }
        }

        static void mouseButtonCallback(purrr_window_t* window, int button, int action, int mods) {
            auto* renderer = WindowRendererMap::get(window);
            if (!renderer) return;

            auto* eventQueue = static_cast<EventQueue*>(purrr_renderer_get_user_pointer(renderer));
            if (!eventQueue) return;

            if (action == GLFW_PRESS) {
                eventQueue->enqueue(std::make_unique<MouseClickEvent>(button));
            }
        }

        static void windowCloseCallback(purrr_window_t* window) {
            auto* renderer = WindowRendererMap::get(window);
            if (!renderer) return;

            auto* eventQueue = static_cast<EventQueue*>(purrr_renderer_get_user_pointer(renderer));
            if (!eventQueue) return;

            eventQueue->enqueue(std::make_unique<WindowCloseEvent>());
        }

        void registerCallbacks(purrr_window_t* window) {
            purrr_set_key_callback(keyCallback);
            purrr_set_mouse_button_callback(mouseButtonCallback);
            purrr_set_window_close_callback(windowCloseCallback);
        }
    };
}

#endif // EVENTS_CALLBACKS_HPP
