#include <window/window.hpp>


namespace robotica::detail {
    void init_sdl(void) {
        static bool init_done = false;
        if (init_done) return; else init_done = true;

        SDL_SetMainReady();
        SDL_Init(SDL_INIT_EVERYTHING);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);


        std::atexit([]() {
            SDL_Quit();
        });
    }
}