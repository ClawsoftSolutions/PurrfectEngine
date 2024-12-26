#ifndef _PURRFECT_ENGINE_EVENTS_EVENT_QUEUE_HPP
#define _PURRFECT_ENGINE_EVENTS_EVENT_QUEUE_HPP

#include <queue>
#include <mutex>
#include <memory>

namespace PurrfectEngine {

  class EventQueue {
  public:
    EventQueue() = default;
    EventQueue(EventQueue &)  = delete;
    EventQueue(EventQueue &&) = delete;

    void enqueue(Event *event);
    void process(EventDispatcher &dispatcher);
  private:
    std::queue<Event*> mEvents;
    std::mutex mEventMutex;
  };

}

#endif // _PURRFECT_ENGINE_EVENTS_EVENT_QUEUE_HPP