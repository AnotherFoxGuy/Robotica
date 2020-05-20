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
        gui_image(void) : gui_image(cv::Mat(1, 1, CV_8UC3)) {}


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

        gui_image(gui_image&&) = default;
        gui_image& operator=(gui_image&&) = default;


        void update(void) {
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cv_image.cols, cv_image.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, cv_image.ptr());
        }


        void set_image(const cv::Mat& img) {
            assert(img.type() == CV_8U || img.type() == CV_8UC3);

            if (img.type() == CV_8U) cv::cvtColor(img, cv_image, cv::COLOR_GRAY2RGBA);
            else cv::cvtColor(img, cv_image, cv::COLOR_BGR2RGBA);

            update();
        }


        cv::Mat get_image(void) {
            cv::Mat result;
            cv::cvtColor(cv_image, result, cv::COLOR_RGBA2BGR);
            return result;
        }


        void show() {
            ImGui::Image((void*) (intptr_t) texture_id, { (float) cv_image.cols, (float) cv_image.rows });
        }
    private:
        cv::Mat cv_image;
        GLuint texture_id;
    };
}