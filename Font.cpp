#include "SDL/include/Renderer.hpp"
#include "SDL/include/Texture.hpp"
#include "SDL/include/Surface.hpp"

#include "debug.hpp"

#include "Font.hpp"

Font::Font(const std::string& font_file, u16_t font_size) : _font(font_file, font_size) { }

sdl::Vector2i Font::getPosition() const {
    return sdl::Vector2i(_clipRect.x, _clipRect.y);
}

void Font::renderOn(const std::string& text, sdl::Renderer& renderer) {
    sdl::Surface srfc = _font.render(text, this->foreground, &this->background, sdl::Font::Solid);

    _clipRect.width = srfc.width();
    _clipRect.height = srfc.height();

    sdl::Texture tex(renderer, srfc);
    renderer.copy(tex, &_clipRect);
}
