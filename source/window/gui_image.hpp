#pragma once

#include <utility/typedefs.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <GL/glew.h>
#include <imgui.h>

#include <cassert>


namespace robotica {
    class gui_image {
    public:
        gui_image(void) : gui_image(cv::Mat(255, 255, CV_8UC3)) {}


        gui_image(cv::Mat cv_image) {
            glGenTextures(1, &texture_id);
            glBindTexture(GL_TEXTURE_2D, texture_id);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            set_image(cv_image);
        }


        ~gui_image(void) {
            glDeleteTextures(1, &texture_id);
        }


        gui_image(const gui_image&) = delete;
        gui_image& operator=(const gui_image&) = delete;


        void update(void) {
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cv_image.cols, cv_image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, cv_image.data);
        }


        void set_image(cv::Mat img) {
            assert(img.type() == CV_8UC3);

            cv_image = img;
            update();
        }


        cv::Mat get_image(void) { return cv_image; }


        void show() {
            ImGui::Image((void*) (intptr_t) texture_id, { (float) cv_image.cols, (float) cv_image.rows });
        }
    private:
        cv::Mat cv_image;
        GLuint texture_id;
    };
}