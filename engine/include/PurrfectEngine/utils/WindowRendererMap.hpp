#ifndef PURFFECT_ENGINE_UTILS_WINDOW_RENDERER_MAP_HPP
#define PURFFECT_ENGINE_UTILS_WINDOW_RENDERER_MAP_HPP

#include <unordered_map>
#include <mutex>
#include "purrr/purrr.h"

class WindowRendererMap {
private:
    static std::unordered_map<purrr_window_t*, purrr_renderer_t*> windowToRendererMap;
    static std::mutex mapMutex;

public:
    static void add(purrr_window_t* window, purrr_renderer_t* renderer) {
        std::lock_guard<std::mutex> lock(mapMutex);
        windowToRendererMap[window] = renderer;
    }

    static purrr_renderer_t* get(purrr_window_t* window) {
        std::lock_guard<std::mutex> lock(mapMutex);
        auto it = windowToRendererMap.find(window);
        return (it != windowToRendererMap.end()) ? it->second : nullptr;
    }

    static void remove(purrr_window_t* window) {
        std::lock_guard<std::mutex> lock(mapMutex);
        windowToRendererMap.erase(window);
    }
};

#endif // WINDOW_RENDERER_MAP_HPP
