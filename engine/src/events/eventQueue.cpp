#include "PurrfectEngine/PurrfectEngine.hpp"

namespace PurrfectEngine {

  void EventQueue::enqueue(Event *event) {
    std::lock_guard<std::mutex> lock(mEventMutex);
    mEvents.push(event);
  }

  void EventQueue::process(EventDispatcher &dispatcher) {
    std::lock_guard<std::mutex> lock(mEventMutex);
    while (!mEvents.empty()) {
      dispatcher.dispatch(*mEvents.front());
      mEvents.pop();
    }
  }

}