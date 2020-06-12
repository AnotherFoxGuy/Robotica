#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <array>
#include <tuple>
#include <cassert>


namespace robotica {
    class target {
    public:
        virtual ~target(void) = default;

        virtual void bind(void) = 0;
        virtual void unbind(void) = 0;
    };


    class screen_target : public target {
    public:
        void bind(void) override {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        }

        void unbind(void) override {
            // Nothing to unbind, since unbinding sets the target to the screen anyway.
        }
    };


    class texture_target : public target {
    public:
        texture_target(glm::vec2 size) : size(size) {
            glGenFramebuffers(1, &framebuffer_id);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer_id);


            std::array textures = {
                std::tuple { &color_id, GL_RGBA,            GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0 },
                std::tuple { &depth_id, GL_DEPTH_COMPONENT, GL_FLOAT,         GL_DEPTH_ATTACHMENT  }
            };


            for (auto& [id, type, format, slot] : textures) {
                glGenTextures(1, id);
                glBindTexture(GL_TEXTURE_2D, *id);

                if (type == GL_RGBA) {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
                }

                glTexImage2D(GL_TEXTURE_2D, 0, type, size.x, size.y, 0, type, format, nullptr);
                glFramebufferTexture(GL_DRAW_FRAMEBUFFER, slot, *id, 0);

                glBindTexture(GL_TEXTURE_2D, 0);
            }

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        }

        ~texture_target(void) {
            if (color_id) glDeleteTextures(1, &color_id);
            if (depth_id) glDeleteTextures(1, &depth_id);
            if (framebuffer_id) glDeleteFramebuffers(1, &framebuffer_id);
        }

        texture_target(const texture_target&) = delete;
        texture_target& operator=(const texture_target&) = delete;

        texture_target(texture_target&& o) {
            *this = std::move(o);
        }

        texture_target& operator=(texture_target&& o) {
            // Easier just to swap the IDs and let the other object delete them.
            // This changes the lifetime, but should have no significant side effects,
            // since doing operations on a moved-out-of object is UB.
            std::swap(framebuffer_id, o.framebuffer_id);
            std::swap(color_id, o.color_id);
            std::swap(depth_id, o.depth_id);
            std::swap(size, o.size);
        }


        void bind(void) override {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer_id);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            GLuint target = GL_COLOR_ATTACHMENT0;
            glDrawBuffers(1, &target);
        }

        void unbind(void) override {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        }


        GLuint get_texture(void) const { return color_id; }
        glm::vec2 get_size(void) const { return size;  }
    private:
        glm::vec2 size;
        GLuint framebuffer_id;
        GLuint color_id = 0, depth_id = 0;


    };
}