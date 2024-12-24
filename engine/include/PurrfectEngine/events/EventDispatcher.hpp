#ifndef   _PURRFECT_ENGINE_EVENTS_EVENT_DISPATCHER_HPP_
#define   _PURRFECT_ENGINE_EVENTS_EVENT_DISPATCHER_HPP_

#include <typeindex>

namespace PurrfectEngine {

  class EventDispatcher {
    struct Listener {
      int priority;
      std::function<void(const Event&)> callback;
    };
  public:
    EventDispatcher() = default;

    template <typename T>
    inline void subscribe(int priority, std::function<void(const T&)> listener) {
      auto& vec = mListeners[typeid(T)];

      auto it = vec.begin();
      while (it < vec.end()) {
        if ((*it).priority < priority) break;
        else ++it;
      }

      vec.insert(it, Listener{
        priority,
        [listener](const Event& e) {
          listener(dynamic_cast<const T&>(e));
        }
      });
    }

    template <typename T>
    inline void dispatch(const T& event) {
      auto it = mListeners.find(typeid(T));
      if (it != mListeners.end()) {
        for (auto& listener : it->second) {
          listener.callback(event);
          if (event.isHandled()) break;
        }
      }
    }
  private:
    std::unordered_map<std::type_index, std::vector<Listener>> mListeners;
  };

}

#endif // _PURRFECT_ENGINE_EVENTS_EVENT_DISPATCHER_HPP_