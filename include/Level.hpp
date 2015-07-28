#ifndef SDL_GAME_LEVEL_HPP
#define SDL_GAME_LEVEL_HPP

#include <vector>
#include "types.hpp"

namespace sdl {
    class Renderer;
    class Texture;

    struct Rect;
}

struct Tile {
    enum ID {
        None,
        Ground,
        Thorn,
        Brittle
    };

    const u16_t id = 0;
    const u16_t mask = 0;

    Tile() = delete;
    explicit Tile(u16_t);
};

class Level {
private:
    sdl::Renderer* _renderer = nullptr;
    std::vector<sdl::Texture*> _textures;
    u16_t _level = 0;

public:
    Level(sdl::Renderer*);

    bool load(u16_t);
    bool loadNext();

    Tile getTileAt(const sdl::Rect&, const sdl::Vector2i&) const;

    u16_t getLevel() const {
        return _level;
    }

    void render();
};

#endif
