#include <window/main_window.hpp>
#include <window/world_image.hpp>
#include <world/robot.hpp>
#include <world/controller.hpp>
#include <utility/utility.hpp>
#include <vision/heat_measure.hpp>
#include <vision/converters.hpp>
#include <vision/world_model.hpp>
#include <comms/websocket.hpp>

#include <opencv2/highgui.hpp>
#include <magic_enum.hpp>

#include <algorithm>
#include <string>
#include <iostream>

#include "dance/MusicAnalyzer.hpp"


namespace robotica {
    using namespace std::string_literals;


    main_window& main_window::instance(void) {
        static main_window i;
        return i;
    }


    main_window::main_window(void) : window("Settings"), lidar_view({ 750, 422 }), lidar_buffer(std::make_shared<buffer>()), path_buffer(std::make_shared<buffer>()) {
        lidar_view.get_program().add_buffer(lidar_buffer);
        lidar_view.get_program().add_buffer(path_buffer);
        

        // Draw a grid to reduce disorientation.
        shared<buffer> grid = std::make_shared<buffer>();
        std::vector<line> lines;
        lines.reserve(100 * 100 * 100);

        constexpr int delta = 20;
        for (int x = -100; x < 100; x += delta) {
            for (int y = -100; y < 100; y += delta) {
                for (int z = -100; z < 100; z += delta) {
                    lines.push_back({ {{ x, y, z }}, {{ x + delta, y, z }} });
                    lines.push_back({ {{ x, y, z }}, {{ x, y + delta, z }} });
                    lines.push_back({ {{ x, y, z }}, {{ x, y, z + delta }} });
                }
            }
        }

        grid->set_data(lines);
        grid->set_color({ 0, 1, 0 });
        grid->set_mode(GL_LINES);
        lidar_view.get_program().add_buffer(grid);
    }


    void main_window::on_frame_start(void) {
        auto& wm = world_model::instance();

        if (enable_graph) {
            std::vector<line> lines;
            for (const auto& edge : wm.get_pathfinding_graph().edges) {
                lines.push_back({
                    *edge.a,
                    *edge.b
                });
            }

            path_buffer->set_data(lines);
        } else {
            path_buffer->set_data(std::vector<vertex>{});
        }

        path_buffer->set_color({ 0, 0, 1 });
        path_buffer->set_mode(GL_LINES);


        lidar_buffer->set_data(wm.get_lidar_pointcloud());
        lidar_buffer->set_mode(GL_POINTS);
        lidar_buffer->set_color({ 1, 0, 0 });


        glPointSize(lidar_point_size);
    }


    void main_window::add_elements(void) {
        constexpr int topbar_height      = 19;
        constexpr int slider_height      = 23;
        constexpr int image_size         = 255;
        constexpr int collapse_width     = 3 * image_size;
        constexpr int padding_top        = 42;
        constexpr int padding_bottom     = 20;
        constexpr int padding_side       = 8;
        constexpr int button_height      = 30;
        constexpr int top_section_height = 420;
        constexpr int sidebar_width      = 350;

        check_input();
        lidar_view.render();


        // Resize window to GUI size.
        ImGui::SetWindowPos({ 0, 0 });
        auto size = ImGui::GetWindowContentRegionMax();
        SDL_SetWindowSize(handle, size.x + 8, size.y + 8);


        // Calculate settings area size.
        const int target_height = padding_top + top_section_height + (2 * image_size) + (2 * padding_bottom);
        const int target_width = std::max(4 * image_size, collapse_width + sidebar_width) + 2 * padding_side;

        if (!has_resized) ImGui::SetWindowSize({ (float) target_width, (float) target_height });
        has_resized = true;


        left.set_image(world_model::instance().get_left_camera());
        right.set_image(world_model::instance().get_right_camera());
        depth.set_image(world_model::instance().get_depth_map());
        map.set_image(generate_world_map());

        // Draw rects around detections.
        // TODO: This needs to be changed to reduce unnecessary conversions.
        for (auto& detection : world_model::instance().get_raw_object_list()) {
            auto& img = left.get_image();

            cv::rectangle(img, detection.bounding_rect, cv::Scalar(0, 0, 255));
            cv::putText(img, detection.type, detection.bounding_rect.tl() + cv::Point{ 2, 15 }, cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 0, 255));

            // Show temperature. Ignore far away pools for accuracy.
            if (detection.type == "Pool" && detection.bounding_rect.height > 4) {
                auto avg_color = std::any_cast<cv::Vec3b>(detection.data);

                float temp = calculate_temperature(avg_color);
                temperature tempclass = temperature_class(temp);

                cv::putText(
                    img,
                    concat(magic_enum::enum_name(tempclass), " (" + std::to_string(temp) + ")"),
                    detection.bounding_rect.tl() + cv::Point { 2, 30 },
                    cv::FONT_HERSHEY_PLAIN,
                    1.0,
                    cv::Scalar(0, 255, 0)
                );
            }

            left.set_image(img);
        }


        // Collapsable groups
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, collapse_width);
        ImGui::SetColumnWidth(1, target_width - collapse_width);

        ImGui::BeginGroup();

        for (const auto& category : collapsable_groups) {
            const std::string name = std::string(magic_enum::enum_name(category));

            if (ImGui::CollapsingHeader(name.c_str())) {
                expand(settings[(int) category], [&](auto& vector) {
                    using type = typename std::remove_reference_t<decltype(vector)>::value_type::type::type;

                    for (auto& elem : vector) {
                        auto& setting = elem.get();

                        // TODO: Add more input types. (notably string and bool.)
                        if constexpr (std::is_integral_v<type>)       ImGui::SliderInt(setting.name.c_str(), &setting.value, setting.min, setting.max);
                        if constexpr (std::is_floating_point_v<type>) ImGui::SliderFloat(setting.name.c_str(), &setting.value, setting.min, setting.max);
                    }
                });
            }
        }

        if (ImGui::CollapsingHeader("music analysis"))
        {
            MusicAnalyzer::instance().view();
        }

        // LIDAR Preview Group
        if (ImGui::CollapsingHeader("LIDAR PREVIEW")) {
            lidar_view.show();

            // Enable capture when scene is clicked.
            if (ImGui::IsItemClicked()) {
                SDL_SetRelativeMouseMode(SDL_TRUE);
                has_capture = true;
            }


            if (ImGui::Button("Goto Cloud", { 200, 30 })) {
                const auto& cloud = world_model::instance().get_lidar_pointcloud();
                if (cloud.size() > 0) lidar_view.get_camera().move_to(cloud[0].position);
            }

            ImGui::SameLine();

            if (ImGui::Button("Toggle Paths", { 200, 30 })) {
                enable_graph = !enable_graph;
            }
        }

        ImGui::EndGroup();
        ImGui::NextColumn();
      

        // Robot Controls
        ImGui::BeginGroup();

        expand(settings[(int) ROBOT], [&](auto& vector) {
            using type = typename std::remove_reference_t<decltype(vector)>::value_type::type::type;

            bool first = true;
            for (auto& elem : vector) {
                auto& setting = elem.get();

                const int h = top_section_height - (2 * button_height + 2 * padding_bottom);
                if constexpr (std::is_integral_v<type>)       ImGui::SliderInt(setting.name.c_str(), &setting.value, setting.min, setting.max);
                if constexpr (std::is_floating_point_v<type>) ImGui::SliderFloat(setting.name.c_str(), &setting.value, setting.min, setting.max);
            }
        });

        // Snapshot buttons
        if (ImGui::Button("Snap Positive", { 200, 30 })) {
            image_to_file(snapshot_folder.string(), "POSITIVE_"s + datetime_string(), left.get_image());
        }

        if (ImGui::Button("Snap Negative", { 200, 30 })) {
            image_to_file(snapshot_folder.string(), "NEGATIVE_"s + datetime_string(), left.get_image());
        }

        if (ImGui::Button("Save Profiler Data", {200, 30})) {
            profiler::dumpBlocksToFile("test_profile.prof");
        }

        if (ImGui::Button("Speak", {200, 30})) {
            //webots::Speaker::playSound( robot::instance().speaker, robot::instance().speaker,"C:/projects/Robotica/protos/kaas.mp3",1,speed,0,false);
            robot::instance().speaker->speak(
                 "I'd just like to interject for moment. What you're refering to as linux, is in fact, gnu/linux, or as I've recently taken to calling it, gnu plus linux. linux is not an operating system unto itself, but rather another free component of a fully functioning gnu system made useful by the gnu corelibs, shell utilities and vital system components comprising a full OS as defined by POSIX.\n"
                "\n"
                "Many computer users run a modified version of the gnu system every day, without realizing it. Through a peculiar turn of events, the version of gnu which is widely used today is often called linux, and many of its users are not aware that it is basically the gnu system, developed by the gnu Project.\n"
                "\n"
                "There really is a linux, and these people are using it, but it is just a part of the system they use. linux is the kernel: the program in the system that allocates the machine's resources to the other programs that you run. The kernel is an essential part of an operating system, but useless by itself; it can only function in the context of a complete operating system. linux is normally used in combination with the gnu operating system: the whole system is basically gnu with linux added, or gnu/linux. All the so-called linux distributions are really distributions of gnu/linux.",
                1
            );
        }

        if (ImGui::Button("emote", {200, 30})) {
            int x = -EMOTICON_WIDTH * (rand() % EMOTICONS_NUMBER_X);
            int y = -EMOTICON_HEIGHT * (rand() % EMOTICONS_NUMBER_Y);
            //wb_display_image_paste(emoticon_display, emoticonsImage, x, y, true);
            robot::instance().display->imagePaste(robot::instance().emotes, x, y, true);
        }

        ImGui::EndGroup();
        ImGui::Columns();

        // Image views
        left.show();
        ImGui::SameLine();
        right.show();
        ImGui::SameLine();
        depth.show();
        ImGui::SameLine();
        map.show();
    }


    void main_window::process_event(SDL_Event* e) {
        if (e->type == SDL_QUIT) controller::instance().request_exit();

        // Disable capture when ESC is pressed.
        if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE && has_capture) {
            SDL_SetRelativeMouseMode(SDL_FALSE);
            has_capture = false;
        }
    }


    void main_window::check_input(void) {
        constexpr float mouse_sens = 0.005;
        constexpr float move_speed = 0.1;
        camera& c = lidar_view.get_camera();

        // SDL unfortunately doesn't provide events every tick,
        // so we just have to check the keyboard state instead.
        if (has_capture) {
            // Mouse motion
            int dx, dy;
            SDL_GetRelativeMouseState(&dx, &dy);

            c.add_yaw(mouse_sens * -dx);
            c.add_pitch(mouse_sens * dy);


            // Keyboard input

            // Move faster when shift is pressed.
            float speed = move_speed;
            if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_LSHIFT]) speed *= 10;

            if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_W]) c.move(camera::absolute_fwd   *  speed);
            if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_S]) c.move(camera::absolute_fwd   * -speed);
            if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_A]) c.move(camera::absolute_right *  speed);
            if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_D]) c.move(camera::absolute_right * -speed);
            if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_Q]) c.move(camera::absolute_up    *  speed);
            if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_E]) c.move(camera::absolute_up    * -speed);
        }
    }
}
