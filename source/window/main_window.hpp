#pragma once

#include <window/window.hpp>
#include <window/setting.hpp>
#include <window/gui_image.hpp>
#include <window/3d/gui_scene.hpp>
#include <utility/traits.hpp>
#include <utility/typedefs.hpp>
#include <utility/math.hpp>

#include <opencv2/core.hpp>
#include <SDL2/SDL_events.h>

#include <type_traits>
#include <filesystem>


#define RBT_SETTING(group, name, default, min, max) setting<decltype(default), container_t> name { *this, #name, default, min, max, (int) group }


namespace robotica {
    const inline fs::path snapshot_folder = ROOT_DIR "/snapshots/";


    class main_window : public window, public setting_storage<main_window> {
    public:
        //constexpr static inline int EMOTICON_WIDTH     = 14;
        //constexpr static inline int EMOTICON_HEIGHT    = 14;
        //constexpr static inline int EMOTICONS_NUMBER_X = 5;
        //constexpr static inline int EMOTICONS_NUMBER_Y = 11;


        static main_window& instance(void);
        main_window(void);

        // To add a new setting category, just create a new entry in this enum, then add it to collapsable_groups.
        // Then you can use RBT_SETTING to add settings to the category.
        enum setting_groups { PARALLAX, CLASSIFIER, ROBOT, ARM, LIDAR, MISC };
        constexpr static auto collapsable_groups = { PARALLAX, CLASSIFIER, ARM, LIDAR };

        // Parallax Processing Settings
        RBT_SETTING(PARALLAX, num_disparities,    3,      0,    16);        // x16
        RBT_SETTING(PARALLAX, block_size,         10,     0,    31);        // x2 + 1
        RBT_SETTING(PARALLAX, disparity_max_diff, 0,      0,    31);
        RBT_SETTING(PARALLAX, speckle_range,      16,     0,    31);
        RBT_SETTING(PARALLAX, speckle_win_size,   2,      2,    127);       // x2 + 1
        RBT_SETTING(PARALLAX, uniqueness_ratio,   0,      0,    32);
        RBT_SETTING(PARALLAX, texture_threshold,  0,      0,    2048);
        RBT_SETTING(PARALLAX, min_disparity,      0,     -128,  128);
        RBT_SETTING(PARALLAX, pre_filter_cap,     1,      1,    63);
        RBT_SETTING(PARALLAX, pre_filter_size,    2,      2,    127);        // x2 + 1
        RBT_SETTING(PARALLAX, lambda,             6000.0, 0,    10000);
        RBT_SETTING(PARALLAX, sigma,              2.0,    0,    10);
        RBT_SETTING(PARALLAX, raw_vis_scale,      21.0,   0,    64);
        RBT_SETTING(PARALLAX, filtered_vis_scale, 15.0,   0,    64);

        // Classifier settings
        RBT_SETTING(CLASSIFIER, min_obj_size,                  5,       1,    255);
        RBT_SETTING(CLASSIFIER, max_obj_size,                  255,     1,    255);
        RBT_SETTING(CLASSIFIER, min_neighbours,                64,      1,    255);
        RBT_SETTING(CLASSIFIER, scale,                         1.07,    1.01, 1.2);
        RBT_SETTING(CLASSIFIER, min_confidence,                1,       0,    255);

        RBT_SETTING(CLASSIFIER, pool_color_threshold,          69,      0,    255);
        RBT_SETTING(CLASSIFIER, pool_min_width,                10,      0,    255);
        RBT_SETTING(CLASSIFIER, pool_min_roundness,            0.0,     0,    1);

        RBT_SETTING(CLASSIFIER, rock_min_size,                 150,     0,    1000);
        RBT_SETTING(CLASSIFIER, rock_min_roundness,            0.255,     0,    1);
        RBT_SETTING(CLASSIFIER, rock_grayness_threshold,       190,     0,    255);

        // LIDAR Settings
        RBT_SETTING(LIDAR,      lidar_scale_factor,            20.0,    1,    100);
        RBT_SETTING(LIDAR,      lidar_point_size,              5.0,     1,    10);
        RBT_SETTING(LIDAR,      lidar_inverse_discard_ratio,   64,      1,    64);
        RBT_SETTING(LIDAR,      lidar_nearby_suppression,      1.15,    0,    2);
        RBT_SETTING(LIDAR,      max_path_length,               1.0,     0,    32);
        RBT_SETTING(LIDAR,      max_path_slope,                1.0,     0,    10);
        RBT_SETTING(LIDAR,      max_path_edges,                10,      1,    32);

        // Robot controls
        RBT_SETTING(ROBOT,    left_motor,         0.25,  -1,    1);
        RBT_SETTING(ROBOT,    right_motor,        0.25,  -1,    1);
        RBT_SETTING(ROBOT,    speed,              100,    1,    100);

        RBT_SETTING(ARM,      arm_base,           0.0,       0,        2 * pi);
        RBT_SETTING(ARM,      arm_short,          0.0,      -3,        0);
        RBT_SETTING(ARM,      arm_long,           0.0,       0,        6);
        RBT_SETTING(ARM,      gripper,            -0.05,    -0.05,     0.7);
        RBT_SETTING(ARM,      gripper_roll,       0.0,      -pi,       +pi);
        RBT_SETTING(ARM,      gripper_pitch,      0.0,      -pi / 2,   +pi / 2);

        // Miscelanneous Settings
        RBT_SETTING(MISC,     enable_graph,     false, false, true);
    protected:
        void add_elements(void) override;
        void process_event(SDL_Event* e) override;
        void on_frame_start(void) override;
    private:
        void check_input(void);

        bool has_resized = false;
        bool has_capture = false;
        gui_image left, right, depth, map;

        gui_scene lidar_view;
        shared<buffer> lidar_buffer, path_buffer;
    };
}
