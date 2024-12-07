#ifndef   _PURRFECT_ENGINE_RENDERER_BUFFER_HPP_
#define   _PURRFECT_ENGINE_RENDERER_BUFFER_HPP_

namespace PurrfectEngine {
namespace renderer {

  class Buffer : public Bindable<uint32_t>, public Wrapper<purrr_buffer_t*> {
  public:
    inline Buffer(purrr_buffer_t *handle)
      : Wrapper(handle)
    {}

    Buffer(Renderer *renderer, BufferType type, uint32_t size);
    ~Buffer();

    bool copy(void *data, uint32_t size, uint32_t offset);
    void *map();
    void unmap();
  private:
    virtual void bind(Renderer *renderer, uint32_t slot) override;
  private:
    uint32_t mSize;
  };

}
}

#endif // _PURRFECT_ENGINE_RENDERER_BUFFER_HPP_