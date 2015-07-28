#ifndef SDL_GAME_DEBUG_HPP
#define SDL_GAME_DEBUG_HPP

#include <iostream>

inline void print() {
    std::cout << "\n";
}

template<typename T, typename ...Types>
inline void print(const T& first, const Types& ...rest) {
    std::cout << first;
    print(rest...);
}

#endif
