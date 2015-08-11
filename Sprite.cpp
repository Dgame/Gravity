#include "SDL/include/Renderer.hpp"
#include "SDL/include/Texture.hpp"
#include "SDL/include/Surface.hpp"

#include "Sprite.hpp"

Sprite::Sprite(const std::string& filename, sdl::Renderer* renderer) {
    sdl::Surface srfc(filename);
    _texture = renderer->createTexture(srfc);

    _clipRect.width  = srfc.width();
    _clipRect.height = srfc.height();
}

sdl::Vector2i Sprite::getEdgePosition(sdl::Edge edge) const {
    return _clipRect.getEdgePosition(edge);
}

sdl::Vector2i Sprite::getCenter() const {
    return _clipRect.getCenter();
}

sdl::Edge Sprite::getMovementEdge() const {
    if (_velocity.x > 0) {
        return sdl::Edge::Right;
    }

    if (_velocity.x < 0) {
        return sdl::Edge::Left;
    }

    if (_velocity.y > 0) {
        return sdl::Edge::Bottom;
    }

    if (_velocity.y < 0) {
        return sdl::Edge::Top;
    }

    return sdl::Edge::TopLeft;
}

void Sprite::drawOn(sdl::Renderer* renderer) {
    _clipRect.x += _velocity.x;
    _clipRect.y += _velocity.y;

    renderer->copy(_texture, &_clipRect);
}
