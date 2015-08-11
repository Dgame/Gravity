// Example program:
// Using SDL2 to create an application window

#include "SDL/include/Window.hpp"
#include "SDL/include/Renderer.hpp"
#include "SDL/include/Event.hpp"
#include "SDL/include/Color.hpp"

#include "SDL/include/Surface.hpp"
#include "SDL/include/Texture.hpp"

#include <iostream>
#include <random>
#include <array>
#include <string>

#include "Level.hpp"
#include "Map.hpp"
#include "Sprite.hpp"
#include "Font.hpp"

#define MOVE 8
#define DEAD_FRAMES 100

std::array<sdl::Vector2i, 16> Fragments;
u16_t DeadFrames = 0;
u16_t Deaths = 0;

std::default_random_engine Gen;
std::uniform_int_distribution<i16_t> Dist(-4, 4);

bool check_tile(Level&, Sprite&);

int main() {
    sdl::Window wnd("Gravity", sdl::Vector2i(100, 100), 640, 480);
    sdl::Renderer* renderer = wnd.createRenderer(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    sdl::Event event;

    Sprite quad("images/rect.png", renderer);
    Level lvl(renderer);

    sdl::Texture* bg = renderer->createTexture("images/background.png");
    sdl::Rect bg_rect(0, 0, wnd.width(), wnd.height());

    Font font("font/arial.ttf", 12);
    font.setPosition(32, wnd.height() - 32);

    bool running = true;
    while (running && lvl.isValid()) {
        while (sdl::PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            else if (event.type == SDL_KEYDOWN) {
                if (!lvl.timer.isRunning())
                    lvl.timer.start();

                switch (event.key.key) {
                    case SDLK_ESCAPE:
                        running = false;
                    break;
                    case SDLK_LEFT:
                        quad.setMovement(-MOVE, 0);
                    break;
                    case SDLK_RIGHT:
                        quad.setMovement(MOVE, 0);
                    break;
                    case SDLK_UP:
                        quad.setMovement(0, -MOVE);
                    break;
                    case SDLK_DOWN:
                        quad.setMovement(0, MOVE);
                    break;
                    case SDLK_SPACE:
                        quad.stopMovement();
                    break;
                }
            }
        }

        renderer->clear(&sdl::Color::White);
        renderer->copy(bg, &bg_rect);

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

            if (DeadFrames == 0) {
                quad.stopMovement();
            }
        }

        lvl.render();

        font.format(renderer, "%d s %d ms, Deaths: %d", lvl.timer.getTicks() / 1000, lvl.timer.getTicks() % 1000, Deaths);

        renderer->present();
    }
}

bool check_tile(Level& lvl, Sprite& quad) {
    const u16_t mask = lvl.getTileFor(quad);
    switch (mask) {
        case Mask::None:
        {
            const sdl::Vector2i pos = quad.getEdgePosition(sdl::Edge::TopLeft);

            if (pos.x > MAP_TILE_WIDTH || pos.x < -(TILE_SIZE * 4)) {
                quad.setPosition(0, 0);
                quad.setMovement(0, 0);
            } else if (pos.y > MAP_TILE_HEIGHT || pos.y < -(TILE_SIZE * 4)) {
                quad.setPosition(0, 0);
                quad.setMovement(0, 0);
            }
        }
        break;

        case Mask::Thorns_Top:
        case Mask::Thorns_Left:
        case Mask::Thorns_Right:
        case Mask::Thorns_Bottom:
        {
            lvl.reload();

            Deaths += 1;

            const sdl::Vector2i center = quad.getCenter();
            const sdl::Vector2i& movement = quad.getMovement();

            for (u16_t i = 0; i < Fragments.size(); i++) {
                const i16_t rx = Dist(Gen);
                const i16_t ry = Dist(Gen);

                const sdl::Vector2i offset(rx + movement.x, ry + movement.y);
                Fragments[i] = center + offset;
            }

            DeadFrames = DEAD_FRAMES;

            quad.setPosition(0, 0);
        }
        break;

        case Mask::Ground:
            quad.setMovement(0, 0);
        break;
    }

    return DeadFrames == 0;
}
