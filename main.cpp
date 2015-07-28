// Example program:
// Using SDL2 to create an application window

#include "SDL/include/Window.hpp"
#include "SDL/include/Renderer.hpp"
#include "SDL/include/Event.hpp"
#include "SDL/include/Color.hpp"

#include <iostream>
#include <random>
#include <array>
#include <string>

#include "Level.hpp"
#include "Map.hpp"
#include "Sprite.hpp"
#include "Font.hpp"

const u16_t Move = 8;

std::array<sdl::Vector2i, 16> Fragments;
u16_t DeadFrames = 0;
u16_t Deaths = 0;

std::default_random_engine Gen;
std::uniform_int_distribution<i16_t> Dist(-4, 4);

bool check_tile(const Level&, Sprite&);

int main() {
    sdl::Window wnd("Gravity", sdl::Vector2i(100, 100), 640, 480);
    sdl::Renderer* renderer = wnd.createRenderer(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    sdl::Event event;

    Sprite quad("images/rect.png", renderer);
    Level lvl(renderer);

    // Font font("font/arial.ttf", 12);

    bool running = true;
    do {
        while (sdl::PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.key) {
                    case SDLK_ESCAPE:
                        running = false;
                    break;
                    case SDLK_LEFT:
                        quad.setMovement(-Move, 0);
                    break;
                    case SDLK_RIGHT:
                        quad.setMovement(Move, 0);
                    break;
                    case SDLK_UP:
                        quad.setMovement(0, -Move);
                    break;
                    case SDLK_DOWN:
                        quad.setMovement(0, Move);
                    break;
                    case SDLK_SPACE:
                        quad.stopMovement();
                    break;
                }
            }
        }

        renderer->clear(&sdl::Color::White);

        if (DeadFrames == 0) {
            if (check_tile(lvl, quad))
                quad.drawOn(renderer);
        } else {
            DeadFrames--;

            renderer->setDrawColor(sdl::Color::Black);
            const sdl::Vector2i& movement = quad.getMovement();

            for (sdl::Vector2i& vec : Fragments) {
                renderer->drawPoint(vec);

                const i16_t rx = Dist(Gen);
                const i16_t ry = Dist(Gen);

                vec.x += rx + movement.x;
                vec.y += ry + movement.y;
            }

            if (DeadFrames == 0)
                quad.stopMovement();
        }

        lvl.renderOn(renderer);

        // font.render("Deaths: " + std::to_string(Deaths));

        renderer->present();
    } while (running);
}

bool check_tile(const Level& lvl, Sprite& quad) {
    const Tile tile = lvl.getTileFor(quad);
    // std::cout << tile.mask << std::endl;
    if (tile.mask == Tile::None) {
        const sdl::Vector2i pos = quad.getEdgePosition(sdl::Edge::TopLeft);

        if (pos.x > MAP_TILE_WIDTH || pos.x < -(TILE_SIZE * 4)) {
            quad.setPosition(0, 0);
            quad.setMovement(0, 0);
        } else if (pos.y > MAP_TILE_HEIGHT || pos.y < -(TILE_SIZE * 4)) {
            quad.setPosition(0, 0);
            quad.setMovement(0, 0);
        }
    } else if (tile.mask == Tile::Thorns) {
        Deaths += 1;

        const sdl::Vector2i center = quad.getCenter();
        const sdl::Vector2i& movement = quad.getMovement();

        for (u16_t i = 0; i < Fragments.size(); i++) {
            const i16_t rx = Dist(Gen);
            const i16_t ry = Dist(Gen);

            const sdl::Vector2i offset(rx + movement.x, ry + movement.y);
            Fragments[i] = center + offset;
        }

        DeadFrames = 100;

        quad.setPosition(0, 0);
    } else if (tile.mask == Tile::Ground) {
        quad.setMovement(0, 0);
    }

    return DeadFrames == 0;
}
