#pragma once

#include <utility/traits.hpp>
#include <utility/typedefs.hpp>
#include <window/gui_image.hpp>
#include <window/setting.hpp>
#include <window/window.hpp>

#include <SDL2/SDL_events.h>
#include <opencv2/core.hpp>

#include <filesystem>
#include <type_traits>

#define RBT_SETTING(group, name, default, min, max) \
    setting<decltype(default), container_t> name { *this, #name, default, min, max, (int)group }

namespace robotica
{
const inline fs::path snapshot_folder = ROOT_DIR "/snapshots/";

class main_window : public window, public setting_storage<main_window>
{
  public:
    static main_window& instance(void);
    main_window(void);

    enum setting_groups
    {
        PARALLAX,
        ROBOT
    };

    // Parallax Processing Settings
    RBT_SETTING(PARALLAX, num_disparities, 3, 0, 16); // x16
    RBT_SETTING(PARALLAX, block_size, 10, 0, 31);     // x2 + 1
    RBT_SETTING(PARALLAX, disparity_max_diff, 0, 0, 31);
    RBT_SETTING(PARALLAX, speckle_range, 16, 0, 31);
    RBT_SETTING(PARALLAX, speckle_win_size, 2, 2, 127); // x2 + 1
    RBT_SETTING(PARALLAX, uniqueness_ratio, 0, 0, 32);
    RBT_SETTING(PARALLAX, texture_threshold, 0, 0, 2048);
    RBT_SETTING(PARALLAX, min_disparity, 0, -128, 128);
    RBT_SETTING(PARALLAX, pre_filter_cap, 1, 1, 63);
    RBT_SETTING(PARALLAX, pre_filter_size, 2, 2, 127); // x2 + 1
    RBT_SETTING(PARALLAX, lambda, 6000.0, 0, 10000);
    RBT_SETTING(PARALLAX, sigma, 2.0, 0, 10);
    RBT_SETTING(PARALLAX, raw_vis_scale, 21.0, 0, 64);
    RBT_SETTING(PARALLAX, filtered_vis_scale, 15.0, 0, 64);

    // Robot controls
    RBT_SETTING(ROBOT, left_motor, 0.0, 0, 1);
    RBT_SETTING(ROBOT, right_motor, 0.0, 0, 1);

    gui_image left, right;
    gui_image left_disp, right_disp, filtered, raw_vis, filtered_vis;

  protected:
    void add_elements(void) override;

  private:
    bool has_resized = false;
};
} // namespace robotica
