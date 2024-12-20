#include "PurrfectEngine/utils/WindowRendererMap.hpp"

std::unordered_map<purrr_window_t*, purrr_renderer_t*> WindowRendererMap::windowToRendererMap;
std::mutex WindowRendererMap::mapMutex;
