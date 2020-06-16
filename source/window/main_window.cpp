#include <window/main_window.hpp>
#include <window/world_image.hpp>
#include <world/robot.hpp>
#include <world/controller.hpp>
#include <utility/utility.hpp>
#include <vision/heat_measure.hpp>
#include <vision/converters.hpp>
#include <vision/world_model.hpp>
#include <comms/websocket.hpp>
#include <window/imgui_plot/imgui_plot.hpp>

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


    main_window::main_window(void) : window("Settings") {}


    void main_window::add_elements(void) {
        constexpr int topbar_height  = 19;
        constexpr int slider_height  = 15;
        constexpr int image_size     = 255;
        constexpr int collapse_width = 3 * image_size;
        constexpr int padding_top    = 42;
        constexpr int padding_bottom = 8;
        constexpr int padding_side   = 8;
        constexpr int button_height  = 30;


        // Count parallax settings.
        int num_elems = 0;
        expand(settings[(int) PARALLAX], [&](const auto& v) { num_elems += std::count_if(v.begin(), v.end(), [](const auto& e) { return e.get().setting_group == (int) PARALLAX; }); });


        // Calculate settings area size.
        const int target_height = padding_top + (slider_height * num_elems) + (2 * image_size) + padding_bottom;
        const int target_width = (4 * image_size) + (5 * padding_side);

        if (!has_resized) ImGui::SetWindowSize({ (float) target_width, (float) target_height });
        has_resized = true;

        // Resize window to GUI size.
        ImGui::SetWindowPos({ 0, 0 });
        auto size = ImGui::GetWindowContentRegionMax();
        SDL_SetWindowSize(handle, size.x + 8, size.y + 8);

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
        ImGui::EndGroup();
        ImGui::NextColumn();
      

        // Robot Controls
        ImGui::BeginGroup();

        expand(settings[(int) ROBOT], [&](auto& vector) {
            using type = typename std::remove_reference_t<decltype(vector)>::value_type::type::type;

            bool first = true;
            for (auto& elem : vector) {
                auto& setting = elem.get();

                if (!first) ImGui::SameLine(); else first = false;

                const int h = slider_height * num_elems - (2 * button_height + 2 * padding_bottom);
                if constexpr (std::is_integral_v<type>)       ImGui::VSliderInt(setting.name.c_str(), ImVec2(slider_height, h), &setting.value, setting.min, setting.max);
                if constexpr (std::is_floating_point_v<type>) ImGui::VSliderFloat(setting.name.c_str(), ImVec2(slider_height, h), &setting.value, setting.min, setting.max);
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

        if (ImGui::Button("Register", {200, 30})) {
            std::string name = websocket::instance().register_bot();
            std::cout << "Registered bot as " << name << '\n';
        }

        if (ImGui::Button("Speak", {200, 30})) {
            //webots::Speaker::playSound( robot::instance().speaker, robot::instance().speaker,"C:/projects/Robotica/protos/kaas.mp3",1,speed,0,false);
            robot::instance().speaker->speak("According to all known laws"
                                             "of aviation,"
                                             "there is no way a bee"
                                             "should be able to fly\n"
                                             "Its wings are too small to get"
                                             "its fat little body off the ground\n"
                                             "The bee, of course, flies anyway"
                                             "because bees don't care"
                                             "what humans think is impossible\n"
                                             "<prosody rate=\"1.5\">"
                                             "Yellow, black. Yellow, black\n"
                                             "Yellow, black. Yellow, black\n"
                                             "Ooh, black and yellow!"
                                             "Let's shake it up a little\n"
                                             "Barry! Breakfast is ready!"
                                             "Ooming!"
                                             "</prosody>",1);

            std::cout << "Registered bot as ";
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
    }
}
