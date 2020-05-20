#pragma once

#include <window/imgui_bindings.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <imgui.h>

#include <string>
#include <cstdlib>


namespace robotica {
    namespace detail {
        inline void assure_sdl_init(void) {
            static bool initialized = false;
            if (initialized) return; else initialized = true;

            SDL_SetMainReady();
            SDL_Init(SDL_INIT_EVERYTHING);

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
            SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

            std::atexit([]() {
                SDL_Quit();
            });
        }
    }


    class window {
    public:
        window(std::string&& name) : name(std::move(name)) {
            detail::assure_sdl_init();

            handle = SDL_CreateWindow(this->name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
            context = SDL_GL_CreateContext(handle);

            SDL_GL_MakeCurrent(handle, context);
            SDL_GL_SetSwapInterval(1);

            glewInit();

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            io = &ImGui::GetIO();

            ImGui::StyleColorsDark();

            ImGui_ImplSDL2_InitForOpenGL(handle, context);
            ImGui_ImplOpenGL3_Init("#version 130");
        }

        virtual ~window(void) {
            close();
        }


        virtual void update(void) {
            poll_events();
            if (closed) return;


            SDL_GL_MakeCurrent(handle, context);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame(handle);
            ImGui::NewFrame();

            if (!ImGui::Begin(name.c_str())) {
                ImGui::End();
                return;
            }

            add_elements();
            ImGui::End();
            ImGui::Render();

            glViewport(0, 0, (int) io->DisplaySize.x, (int) io->DisplaySize.y);

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(handle);
        }


        virtual void close(void) {
            if (closed) return; else closed = true;

            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplSDL2_Shutdown();
            ImGui::DestroyContext();

            SDL_GL_DeleteContext(context);
            SDL_DestroyWindow(handle);
        }
    protected:
        virtual void add_elements(void) = 0;
        virtual void process_event(SDL_Event*) {} 


        SDL_Window* handle;
        SDL_GLContext context;
        ImGuiIO* io;

    private:
        void poll_events(void) {
            if (closed) return;

            SDL_Event e;

            while (SDL_PollEvent(&e)) {
                ImGui_ImplSDL2_ProcessEvent(&e);
                process_event(&e);
                if (e.type == SDL_QUIT) close();
            }
        }


        std::string name;
        bool closed = false;
    };
}