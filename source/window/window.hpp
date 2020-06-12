#pragma once

#include <window/imgui_bindings.hpp>
#include <window/3d/gui_scene.hpp>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <imgui.h>

#include <string>
#include <cstdlib>

#include <window/3d/program.hpp>
#include <window/3d/primitive.hpp>


namespace robotica {
    namespace detail {
        extern void init_sdl(void);
    }


    class window {
    public:
        window(std::string&& name) : name(std::move(name)) {
            detail::init_sdl();

            handle = SDL_CreateWindow(this->name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
            context = SDL_GL_CreateContext(handle);

            SDL_GL_SetSwapInterval(1);

            glewInit();


            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            io = &ImGui::GetIO();
            
            ImGui::StyleColorsDark();
            
            ImGui_ImplSDL2_InitForOpenGL(handle, context);
            ImGui_ImplOpenGL3_Init("#version 330");


            glClearColor(0.5, 0.5, 0.5, 1.0);
            glEnable(GL_DEPTH_TEST);
        }

        virtual ~window(void) {
            close();
        }


        virtual void update(void) {
            EASY_BLOCK("window update", profiler::colors::Red);

            poll_events();
            if (closed) return;

            //glViewport(0, 0, (int) io->DisplaySize.x, (int) io->DisplaySize.y);
            
            int w, h;
            SDL_GL_GetDrawableSize(handle, &w, &h);
            glViewport(0, 0, w, h);


            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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


        void register_scene(gui_scene* scene) {
            
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
                process_event(&e);

                if (e.type == SDL_QUIT) {
                    close();
                    break;
                }

                ImGui_ImplSDL2_ProcessEvent(&e);
            }
        }


        std::string name;
        bool closed = false;
    };
}