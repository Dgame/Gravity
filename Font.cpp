// #include "SDL/include/Font.hpp"
// #include "SDL/include/Renderer.hpp"
// #include "SDL/include/Texture.hpp"
// #include "SDL/include/Surface.hpp"
//
// #include "Font.hpp"
//
// Font::Font(const std::string& font_file, u16_t font_size) : _font(font_file, font_size) { }
//
// sdl::Vector2i Font::getPosition() const {
//     return sdl::Vector2i(_clipRect.x, _clipRect.y);
// }
//
// void Font::render(const std::string& text, sdl::Renderer* renderer) {
//     sdl::Surface srfc = _font.render(text, this->foreground, &this->background, this->mode);
//     sdl::Texture* tex = srfc.asTextureOf(renderer);
//
//     renderer->copy(tex, &_clipRect);
// }
