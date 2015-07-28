#ifndef SDL_GAME_LEVEL_HPP
#define SDL_GAME_LEVEL_HPP

#include <vector>
#include "types.hpp"

#include "SDL/include/Vector2.hpp"

namespace sdl {
    class Renderer;
    class Texture;

    struct Rect;
}

class Sprite;

struct Tile {
    enum ID {
        None,
        Brittle,
        Thorns,
        Ground,
    };

    const u16_t id = 0;
    const u16_t mask = 0;

    Tile() = delete;
    explicit Tile(u16_t);
};

class Level {
private:
    std::vector<sdl::Texture*> _textures;
    u16_t _level = 0;

public:
    explicit Level(sdl::Renderer*);

    bool load(u16_t);
    bool loadNext();

    Tile getTileFor(const Sprite&) const;

    u16_t getLevel() const {
        return _level;
    }

    void renderOn(sdl::Renderer*);
};

#endif
