#ifndef PURRFECT_ENGINE_EVENTS_EVENT_DISPATCHER_HPP
#define PURRFECT_ENGINE_EVENTS_EVENT_DISPATCHER_HPP

#include "event.hpp"
#include <unordered_map>
#include <vector>
#include <functional>
#include <typeindex>
#include <algorithm>

namespace PurrfectEngine {
    class EventDispatcher {
        struct Listener {
            int priority;
            std::function<void(const Event&)> callback;
        };

        std::unordered_map<std::type_index, std::vector<Listener>> listeners;

    public:
        EventDispatcher() {};
        ~EventDispatcher() {};

        template <typename T>
        void subscribe(int priority, std::function<void(const T&)> listener) {
            auto& vec = listeners[typeid(T)];
            vec.push_back({ priority, [listener](const Event& e) {
                listener(static_cast<const T&>(e));
            } });

            std::sort(vec.begin(), vec.end(), [](const Listener& a, const Listener& b) {
                return a.priority > b.priority;
                });
        }

        template <typename T>
        void dispatch(const T& event) {
            auto it = listeners.find(typeid(T));
            if (it != listeners.end()) {
                for (auto& listener : it->second) {
                    listener.callback(event);
                    if (event.isHandled()) break;
                }
            }
        }
    };
}

#endif // !PURRFECT_ENGINE_EVENTS_EVENT_DISPATCHER_HPP
