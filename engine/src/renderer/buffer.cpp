#include "PurrfectEngine/PurrfectEngine.hpp"

namespace PurrfectEngine::renderer {

  Buffer::Buffer(Renderer *renderer, BufferType type, uint32_t size)
    : mSize(size)
  {
    purrr_buffer_info_t info{
      (purrr_buffer_type_t)type, size
    };

    mHandle = purrr_buffer_create(&info, renderer->getRenderer());
    assert(mHandle);
  }

  Buffer::~Buffer() {
    purrr_buffer_destroy(mHandle);
  }

  bool Buffer::copy(void *data, uint32_t size, uint32_t offset) {
    return purrr_buffer_copy(mHandle, data, size, offset);
  }

  void *Buffer::map() {
    void *data = nullptr;
    if (!purrr_buffer_map(mHandle, &data)) return nullptr;
    return data;
  }

  void Buffer::unmap() {
    purrr_buffer_unmap(mHandle);
  }

  void Buffer::bind(Renderer *renderer, uint32_t slot) {
    purrr_renderer_bind_buffer(renderer->getRenderer(), mHandle, slot);
  }

}