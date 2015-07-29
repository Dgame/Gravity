#include <string>
#include <string.h>
#include <cmath>
#include <limits>
#include <fstream>
#include <sstream>

#include "debug.hpp"

#include "Level.hpp"
#include "Map.hpp"
#include "Sprite.hpp"

#include "SDL/include/Renderer.hpp"
#include "SDL/include/Surface.hpp"
#include "SDL/include/Texture.hpp"
#include "SDL/include/Rect.hpp"
#include "SDL/include/Vector2.hpp"

#define NEED_VISIT 2

const std::string Tileset = "tileset=";
const std::string Data = "data=";

std::string LevelFileFor(u16_t lvl) {
    std::stringstream ss;
    ss << "Level/level" << lvl << ".txt";

    return ss.str();
}

Level::Level(sdl::Renderer* renderer) : _renderer(renderer) {
    _map.reserve(MAP_TILES);
    this->load(1);
}

void Level::load(u16_t lvl) {
    _level = lvl;

    const std::string level_file = LevelFileFor(lvl);

    std::ifstream is(level_file);
    if (!is) {
        _valid = false;
        return;
    }

    _valid = true;
    u16_t img_id = 0;

    std::string line;
    while (is) {
        std::getline(is, line);

        auto s = line.find(Tileset);
        if (s != std::string::npos) {
            img_id++;

            const u16_t tc = Tileset.size();
            s += tc;
            auto e = line.find(',', s);

            const std::string img = line.substr(s, e - tc);
            if (img_id > _textures.size()) {
                sdl::Surface srfc(img);
                _textures.push_back(srfc.asTextureOf(_renderer));
            }
        }

        if (line == Data) {
            break;
        }
    }

    if (_map.size() > 0)
        _map.clear();

    while (is) {
        std::getline(is, line);
        const u16_t c = line.length();
        for (u16_t i = 0; i < c; i++) {
            u16_t id = 0;
            while (line[i] != ',' && i < c) {
                id *= 10;
                id += line[i] - '0';

                i++;
            }
            _map.push_back(id);
        }
    }
}

void Level::loadNext() {
    this->load(_level + 1);
}

u16_t Level::getTileID(u16_t x, u16_t y) const {
    const u16_t index = y * MAP_HEIGHT + x;
    if (index >= MAP_TILES)
        return 0;
    return _map[index];
}

void Level::setTileID(u16_t x, u16_t y, u16_t id) {
    const u16_t index = y * MAP_HEIGHT + x;
    if (index < MAP_TILES) {
        _map[index] = id;
    }
}

void Level::reload() {
    _visited = 0;

    for (u16_t& id : _map) {
        if (id == Mask::Visited) {
            id = Mask::Not_Visited;
        }
    }
}

u16_t Level::getTileFor(const Sprite& quad) {
    const sdl::Vector2i& movement = quad.getMovement();
    const sdl::Edge edge = quad.getMovementEdge();

    sdl::Vector2i edge_pos = quad.getEdgePosition(edge);

    if (movement.x < 0)
        edge_pos.x -= 1;
    else if (movement.y < 0)
        edge_pos.y -= 1;

    edge_pos /= TILE_SIZE;

    sdl::Vector2f pos = quad.getEdgePosition(sdl::Edge::TopLeft);
    pos /= static_cast<f32_t>(TILE_SIZE);

    u16_t mask = this->getTileID(edge_pos.x, edge_pos.y);
/*
    const f32_t dx = pos.x - static_cast<i16_t>(pos.x);
    const f32_t dy = pos.y - static_cast<i16_t>(pos.y);
    const sdl::Vector2i delta(dy < 0 ? 1 : -1, dx > 0 ? 1 : -1);

    if (movement.y != 0 && !sdl::CompareFloats(dx, 0)) {
        const u16_t m = this->getTileID(edge_pos.x + delta.x, edge_pos.y + delta.y);
        print("dx = ", dx, ", m = ", m);

        if (m == Mask::Thorns_Left || m == Mask::Thorns_Right) {
            return m;
        }
    } else if (movement.x != 0 && !sdl::CompareFloats(dy, 0)) {
        const u16_t m = this->getTileID(edge_pos.x + delta.x, edge_pos.y + delta.y);
        print("dy = ", dy, ", m = ", m);

        if (m == Mask::Thorns_Left || m == Mask::Thorns_Right) {
            return m;
        }
    }
*/
    if (mask == Mask::Not_Visited) {
        _visited++;
        this->setTileID(edge_pos.x, edge_pos.y, Mask::Visited);
    } else if (mask == Mask::Start && _visited >= NEED_VISIT) {
        this->loadNext();
    }

    return mask;
}

void Level::render() {
    sdl::Rect dst(0, 0, TILE_SIZE, TILE_SIZE);

    for (u16_t id : _map) {
        if (id > 0) {
            sdl::Texture* tex = _textures[id - 1];
            _renderer->copy(tex, &dst);
        }

        dst.x += TILE_SIZE;
        if (dst.x >= MAP_TILE_WIDTH) {
            dst.x = 0;
            dst.y += TILE_SIZE;
        }
    }
}
