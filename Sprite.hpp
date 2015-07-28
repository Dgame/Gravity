#ifndef SDL_GAME_SPRITE_HPP
#define SDL_GAME_SPRITE_HPP

#include <string>

#include "SDL/include/Vector2.hpp"
#include "SDL/include/Rect.hpp"

namespace sdl {
    class Renderer;
    class Texture;
}

class Sprite {
private:
    sdl::Vector2i _velocity;
    sdl::Rect _clipRect;
    sdl::Texture* _texture = nullptr;

public:
    explicit Sprite(const std::string&, sdl::Renderer*);

    void setPosition(i16_t x, i16_t y) {
        _clipRect.x = x;
        _clipRect.y = y;
    }

    sdl::Vector2i getCenter() const;

    sdl::Vector2i getEdgePosition(sdl::Edge) const;
    sdl::Edge getMovementEdge() const;

    void setMovement(i16_t dx, i16_t dy) {
        _velocity.x = dx;
        _velocity.y = dy;
    }

    void stopMovement() {
        _velocity.x = _velocity.y = 0;
    }

    const sdl::Vector2i& getMovement() const {
        return _velocity;
    }

    void drawOn(sdl::Renderer*);
};

#endif
