#pragma once

#include <window/3d/primitive.hpp>

#include <GL/glew.h>

#include <vector>
#include <cstddef>
#include <array>


namespace robotica {
    class buffer {
    public:
        buffer(void) : id(next_id++) {
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            glGenBuffers(1, &vbo);
        }

        template <typename T> buffer(const std::vector<T>& data) : buffer() {
            set_data(data);
        }


        buffer(const buffer&) = delete;
        buffer& operator=(const buffer&) = delete;


        buffer(buffer&& o) {
            *this = std::move(o);
        }

        buffer& operator=(buffer&& o) {
            if (vao) glDeleteVertexArrays(1, &vbo);
            if (vbo) glDeleteBuffers(1, &vbo);


            id    = o.id;
            size  = o.size;
            vao   = o.vao;
            vbo   = o.vbo;
            o.vao = o.vbo = 0;

            return *this;
        }

        ~buffer(void) {
            if (vao) glDeleteVertexArrays(1, &vbo);
            if (vbo) glDeleteBuffers(1, &vbo);
        }


        void bind(GLuint program) {
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);


            GLint index = glGetAttribLocation(program, "position");
            assert(index != -1);

            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) 0);


            glUniform3fv(glGetUniformLocation(program, "color"), 1, glm::value_ptr(color));
        }


        // Note that vertex attributes will be interpreted as an array of vec3 positions,
        // regardless of the provided type.
        template <typename T> void set_data(const std::vector<T>& data) {
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            if (data.size()) glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data.size(), &data[0], GL_STATIC_DRAW);
            else glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

            size = (sizeof(T) / sizeof(vertex)) * data.size();
        }


        void set_color(glm::vec3 color) { this->color = color; }


        void set_mode(GLenum mode) { this->mode = mode; }
        GLenum get_mode(void) const { return mode; }


        std::size_t get_id(void) const { return id; }
        std::size_t get_size(void) const { return size; }
    private:
        static inline std::size_t next_id = 0;

        GLuint vao = 0, vbo = 0;
        GLenum mode = GL_TRIANGLES;
        glm::vec3 color = { 1, 0, 0 };
        std::size_t size;
        std::size_t id;
    };
}