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

namespace Mask {
    enum ID {
        None          = 0,
        Brittle       = 1,
        Thorns_Bottom = 2,
        Thorns_Left   = 3,
        Thorns_Right  = 4,
        Thorns_Top    = 5,
        Ground        = 6,
        Not_Visited   = 7,
        Visited       = 8,
        Start         = 9,
    };
}

inline bool IsThorn(u16_t id) {
    return id >= Mask::Thorns_Bottom && id <= Mask::Thorns_Top;
}

std::string LevelFileFor(u16_t);

class Level {
private:
    std::vector<sdl::Texture*> _textures;
    std::vector<u16_t> _map;

    sdl::Renderer* _renderer;

    u16_t _level = 0;
    u16_t _visited = 0;

    bool _valid = false;

    void load(u16_t);
    void loadNext();

    u16_t getTileID(u16_t, u16_t) const;
    void setTileID(u16_t, u16_t, u16_t);

public:
    explicit Level(sdl::Renderer*);

    bool isValid() const {
        return _valid;
    }

    u16_t getLevel() const {
        return _level;
    }

    u16_t getVisited() const {
        return _visited;
    }

    void reload();

    u16_t getTileFor(const Sprite&);

    void render();
};
#endif
