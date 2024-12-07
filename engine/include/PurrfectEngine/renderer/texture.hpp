#ifndef   _PURRFECT_ENGINE_RENDERER_TEXTURE_HPP_
#define   _PURRFECT_ENGINE_RENDERER_TEXTURE_HPP_

namespace PurrfectEngine {
namespace renderer {

  class Texture : public Bindable<uint32_t> {
  public:
    Texture(Renderer *renderer, Format format, uint32_t width, uint32_t height);
    Texture(Renderer *renderer, Format format, const char *filename);
    ~Texture();

    // bool copy();
  private:
    virtual void bind(Renderer *renderer, uint32_t slot) override;
  private:
    purrr_texture_t *mTexture = nullptr;
  };

}
}

#endif // _PURRFECT_ENGINE_RENDERER_TEXTURE_HPP_