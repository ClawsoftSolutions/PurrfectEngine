#ifndef PURRFECT_ENGINE_EVENTS_EVENT_QUEUE_HPP
#define PURRFECT_ENGINE_EVENTS_EVENT_QUEUE_HPP

#include "EventDispatcher.hpp"
#include <queue>
#include <mutex>
#include <memory>

namespace PurrfectEngine {
    class EventQueue {
        std::queue<std::unique_ptr<Event>> events;
        std::mutex eventMutex;

    public:
        EventQueue() {};
        ~EventQueue() {};

        void enqueue(std::unique_ptr<Event> event) {
            std::lock_guard<std::mutex> lock(eventMutex);
            events.push(std::move(event));
        }

        void process(EventDispatcher& dispatcher) {
            std::lock_guard<std::mutex> lock(eventMutex);
            while (!events.empty()) {
                dispatcher.dispatch(*events.front());
                events.pop();
            }
        }
    };
}

#endif PURRFECT_ENGINE_EVENTS_EVENT_QUEUE_HPP