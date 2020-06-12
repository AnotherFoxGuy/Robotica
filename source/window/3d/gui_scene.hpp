#pragma once

#include <utility/typedefs.hpp>
#include <window/3d/program.hpp>
#include <window/3d/target.hpp>
#include <window/3d/camera.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>


namespace robotica {
    class gui_scene {
    public:
        gui_scene(glm::vec2 size) :
            tgt(std::make_shared<texture_target>(size)),
            render_program(std::move(program("default", tgt))),
            render_camera(size.x / size.y)
        {}


        gui_scene(const gui_scene&) = delete;
        gui_scene& operator=(const gui_scene&) = delete;

        gui_scene(gui_scene&&) = default;
        gui_scene& operator=(gui_scene&&) = default;


        void render(void) {
            render_program.set_camera(render_camera.get_matrix());
            render_program.execute();
        }


        void show(void) {
            ImGui::Image((void*) (intptr_t) tgt->get_texture(), { tgt->get_size().x, tgt->get_size().y });
        }


        program& get_program(void) { return render_program; }
        camera& get_camera(void) { return render_camera; }
    private:
        shared<texture_target> tgt;
        program render_program;
        camera render_camera;
    };
}