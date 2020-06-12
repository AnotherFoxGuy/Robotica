#pragma once

#include <utility/utility.hpp>
#include <utility/typedefs.hpp>
#include <window/3d/buffer.hpp>
#include <window/3d/target.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string_view>
#include <string>
#include <fstream>
#include <array>
#include <tuple>
#include <vector>


namespace robotica {
    namespace detail {
        const inline fs::path shader_directory = ROOT_DIR "/assets/shaders/";


        inline std::string load_file(std::string_view path) {
            std::ifstream stream(path);

            std::string line, result;
            while (std::getline(stream, line)) result += line + "\r\n";

            return result;
        }


        inline GLuint load_shader(std::string_view vertfile, std::string_view fragfile) {
            std::array data {
                std::tuple { (GLuint) 0, vertfile, GL_VERTEX_SHADER   },
                std::tuple { (GLuint) 0, fragfile, GL_FRAGMENT_SHADER }
            };


            for (auto& [id, file, type] : data) {
                std::string src = load_file((shader_directory / file).string());
                const GLchar* srcptr = src.c_str();
                const GLint   srclen = src.length();

                id = glCreateShader(type);
                glShaderSource(id, 1, &srcptr, &srclen);
                glCompileShader(id);
            }


            GLuint program = glCreateProgram();
            for (auto& [id, file, type] : data) glAttachShader(program, id);
            glLinkProgram(program);

            // Unlike what the name suggests, shaders aren't actually deleted until glDeleteProgram is called.
            for (auto& [id, file, type] : data) {
                glDetachShader(program, id);
                glDeleteShader(id);
            }


            return program;
        }
    }


    class program {
    public:
        program(void) = default;

        program(const program&) = delete;
        program& operator=(const program&) = delete;

        program(program&& o) {
            *this = std::move(o);
        }

        program& operator=(program&& o) {
            if (id) glDeleteProgram(id);

            name = std::move(o.name);
            tgt  = std::move(o.tgt);
            id   = o.id;
            o.id = 0;

            return *this;
        }

        program(std::string_view name, shared<target> tgt) :
            id(detail::load_shader(concat(name, ".vert.glsl"), concat(name, ".frag.glsl"))),
            tgt(std::move(tgt))
        {}

        ~program(void) {
            if (id) glDeleteProgram(id);
        }


        void execute(void) {
            tgt->bind();

            glUseProgram(id);
            for (auto& buf : buffers) {
                buf.bind(id);
                glDrawArrays(buf.get_mode(), 0, buf.get_size());
            }

            tgt->unbind();
        }


        void add_buffer(buffer&& buf) {
            buffers.push_back(std::move(buf));
        }


        void remove_buffer(std::size_t id) {
            remove_if(buffers, [&](const auto& buf) { return buf.get_id() == id; });
        }


        void set_camera(const glm::mat4& camera) {
            glUseProgram(id);
            glUniformMatrix4fv(glGetUniformLocation(id, "camera"), 1, GL_FALSE, glm::value_ptr(camera));
        }
    private:
        std::string name;
        shared<target> tgt;
        GLuint id = 0;

        std::vector<buffer> buffers;
    };
}