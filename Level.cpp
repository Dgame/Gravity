#include <string>
#include <cmath>
#include <limits>

#include "debug.hpp"

#include "Level.hpp"
#include "LevelMaps.hpp"
#include "Sprite.hpp"

#include "SDL/include/Renderer.hpp"
#include "SDL/include/Surface.hpp"
#include "SDL/include/Texture.hpp"
#include "SDL/include/Rect.hpp"
#include "SDL/include/Vector2.hpp"

const std::string TileImages[] = {
    "ground_brittle.png",
    "thorns_bottom.png",
    "thorns_left.png",
    "thorns_right.png",
    "thorns_top.png",
    "ground.png",
};

const u16_t TileMaks[] = {
    Tile::None,
    Tile::Brittle,
    Tile::Thorns,
    Tile::Thorns,
    Tile::Thorns,
    Tile::Thorns,
    Tile::Ground
};

const std::string Path = "images/";

Tile::Tile(u16_t my_id) : id(my_id), mask(TileMaks[my_id]) { }

Level::Level(sdl::Renderer* renderer) {
    for (const std::string& file : TileImages) {
        sdl::Surface srfc(Path + file);
        _textures.push_back(srfc.asTextureOf(renderer));
    }
}

bool Level::load(u16_t lvl) {
    if (lvl >= MAPS)
        return false;

    _level = lvl;

    return true;
}

bool Level::loadNext() {
    return this->load(_level + 1);
}

Tile Level::getTileFor(const Sprite& quad) const {
    const sdl::Edge edge = quad.getMovementEdge();
    const sdl::Vector2i edge_pos = quad.getEdgePosition(edge) / TILE_SIZE;
    sdl::Vector2f pos = quad.getEdgePosition(sdl::Edge::TopLeft);

    // pos.x /= TILE_SIZE;
    // pos.y /= TILE_SIZE;

    u16_t mask = getTileID(_level, edge_pos.x, edge_pos.y);

    auto fcmp = [](f32_t a, f32_t b) {
        return std::abs(a - b) < std::numeric_limits<f32_t>::epsilon();
    };

    const f32_t dx = pos.x - static_cast<i16_t>(pos.x);
    const f32_t dy = pos.y - static_cast<i16_t>(pos.y);

    print(edge_pos.x, ':', edge_pos.y, " = ", mask, " -> ", pos.x, ':', pos.y, ", dx = ", dx, ", dy = ", dy);

    if (!fcmp(dx, 0)) {
        const i16_t delta = dx > 0 ? 1 : -1;
        const u16_t m = getTileID(_level, edge_pos.x + delta, edge_pos.y);
        if (m > 0 && m < mask)
            mask = m;
        print("Korrigiere nach dx");
    } else if (!fcmp(dy, 0)) {
        const i16_t delta = dy > 0 ? 1 : -1;
        const u16_t m = getTileID(_level, edge_pos.x, edge_pos.y + delta);
        if (m > 0 && m < mask)
            mask = m;
        print("Korrigiere nach dy");
    }

    return Tile(mask);
}

void Level::renderOn(sdl::Renderer* renderer) {
    sdl::Rect dst(0, 0, TILE_SIZE, TILE_SIZE);

    for (u16_t t : LevelMaps[_level]) {
        if (t > 0) {
            sdl::Texture* tex = _textures[t - 1];
            renderer->copy(tex, &dst);
        }

        dst.x += TILE_SIZE;
        if (dst.x >= MAP_TILE_WIDTH) {
            dst.x = 0;
            dst.y += TILE_SIZE;
        }
    }
}
