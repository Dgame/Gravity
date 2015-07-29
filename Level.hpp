#ifndef SDL_GAME_LEVEL_HPP
#define SDL_GAME_LEVEL_HPP

#include <string>
#include <vector>
#include "types.hpp"

#include "SDL/include/Vector2.hpp"

namespace sdl {
    class Renderer;
    class Texture;

    struct Rect;
}

class Sprite;

enum class Mask {
    None          = 0,
    // Brittle       = 1,
    Thorns_Bottom = 2,
    Thorns_Left   = 3,
    Thorns_Right  = 4,
    Thorns_Top    = 5,
    Ground        = 6,
    // Not_Visited   = 7,
    // Visited       = 8,
};

std::string LevelFileFor(u16_t);

class Level {
private:
    std::vector<sdl::Texture*> _textures;
    std::vector<u16_t> _map;

    sdl::Renderer* _renderer;

    u16_t _level = 0;

public:
    explicit Level(sdl::Renderer*);

    bool load(u16_t);
    bool loadNext();

    u16_t getTileID(u16_t, u16_t) const;
    Mask getTileFor(const Sprite&) const;

    u16_t getLevel() const {
        return _level;
    }

    void render();
};
#endif
