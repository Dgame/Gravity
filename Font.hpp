#ifndef SDL_TEXT_HPP
#define SDL_TEXT_HPP

#include <string>
#include <memory>

#include "SDL/include/Rect.hpp"
#include "SDL/include/Vector2.hpp"
#include "SDL/include/Color.hpp"
#include "SDL/include/Font.hpp"

namespace sdl {
    class Font;
    class Renderer;
    class Texture;
}

class Font {
private:
    sdl::Font _font;
    sdl::Rect _clipRect;

public:
    sdl::Color foreground = sdl::Color::Black;
    sdl::Color background = sdl::Color::White;

    explicit Font(const std::string&, u16_t);
    Font(const Font&) = delete;

    void setPosition(i16_t x, i16_t y) {
        _clipRect.x = x;
        _clipRect.y = y;
    }

    sdl::Vector2i getPosition() const;

    template <typename ...Arguments>
    void format(sdl::Renderer*, const std::string&, Arguments&&...);

    void renderOn(const std::string&, sdl::Renderer*);

    const sdl::Font* getFont() const {
        return &_font;
    }
};

template <typename ...Arguments>
void Font::format(sdl::Renderer* renderer, const std::string& fmt, Arguments&& ...args) {
    const u32_t size = snprintf(nullptr, 0, fmt.c_str(), args...) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf(new char[size]);

    snprintf(buf.get(), size, fmt.c_str(), args...);

    const std::string text = std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
    this->renderOn(text, renderer);
}

#endif
