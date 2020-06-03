#include <window/main_window.hpp>
#include <window/world_image.hpp>
#include <world/robot.hpp>
#include <utility/utility.hpp>
#include <vision/parallax.hpp>
#include <vision/converters.hpp>
#include <vision/classifier.hpp>
#include <vision/world_model.hpp>

#include <opencv2/highgui.hpp>

#include <algorithm>
#include <string>


namespace robotica {
    using namespace std::string_literals;


    main_window& main_window::instance(void) {
        static main_window i;
        return i;
    }


    main_window::main_window(void) : window("Settings") {}


    void main_window::add_elements(void) {
        constexpr int topbar_height  = 19;
        constexpr int slider_height  = 23;
        constexpr int image_size     = 255;
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
            left.set_image(img);
        }


        // Parallax Settings
        ImGui::BeginGroup();

        expand(settings[(int) PARALLAX], [&](auto& vector) {
            using type = typename std::remove_reference_t<decltype(vector)>::value_type::type::type;

            for (auto& elem : vector) {
                auto& setting = elem.get();

                // TODO: Add more input types. (notably string and bool.)
                if constexpr (std::is_integral_v<type>)       ImGui::SliderInt(setting.name.c_str(), &setting.value, setting.min, setting.max);
                if constexpr (std::is_floating_point_v<type>) ImGui::SliderFloat(setting.name.c_str(), &setting.value, setting.min, setting.max);
            }
        });

        ImGui::EndGroup();


        // Robot Controls
        ImGui::SameLine();
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

        if (ImGui::Button("profiler dumpBlocksToFile", {200, 30}))
        {
            profiler::dumpBlocksToFile("test_profile.prof");
        }

        ImGui::EndGroup();


        // Image views
        left.show();
        ImGui::SameLine();
        right.show();
        ImGui::SameLine();
        depth.show();
        ImGui::SameLine();
        map.show();
    }
}