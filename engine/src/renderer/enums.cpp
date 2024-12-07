#include "PurrfectEngine/PurrfectEngine.hpp"

namespace PurrfectEngine::renderer {

  uint32_t formatSize(Format format) {
    switch (format) {
    case Format::GRAYSCALE:  return 1;
    case Format::GRAY_ALPHA: return 2;
    case Format::RGBA8U:     return 4;
    case Format::RGBA8RGB:   return 4;
    case Format::BGRA8U:     return 4;
    case Format::BGRA8RGB:   return 4;
    case Format::RGBA16F:    return 4;
    case Format::RG32F:      return 2;
    case Format::RGB32F:     return 3;
    case Format::RGBA32F:    return 4;
    case Format::RGBA64F:    return 4;
    }
    assert(0 && "Unreachable");
  }

}