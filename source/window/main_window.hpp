#pragma once

#include <window/window.hpp>
#include <window/gui_image.hpp>
#include <window/setting.hpp>
#include <utility/traits.hpp>

#include <opencv2/core.hpp>
#include <SDL2/SDL_events.h>

#include <type_traits>

#define RBT_SETTING(name, default, min, max) setting<decltype(default), container_t> name { *this, #name, default, min, max }


namespace robotica {
    class main_window : public window, public setting_storage<main_window> {
    public:
        static main_window& instance(void);
        main_window(void);


        // Parallax Processing Settings
        // TODO: Move settings to seperate class.
        RBT_SETTING(num_disparities,    1,   0,    16);     // x 16
        RBT_SETTING(block_size,         2,   0,    32);     // x 2 + 1
        RBT_SETTING(disparity_max_diff, 1,   0,    32);
        RBT_SETTING(speckle_range,      8,   0,    32);
        RBT_SETTING(speckle_win_size,   2,   2,    127);     // x2 + 1
        RBT_SETTING(uniqueness_ratio,   0,   0,    32);
        RBT_SETTING(texture_threshold,  507, 0,    2048);
        RBT_SETTING(min_disparity,      -39, -128, 128);
        RBT_SETTING(pre_filter_cap,     61,  1,    63);
        RBT_SETTING(pre_filter_size,    5,   0,    32);
        RBT_SETTING(sad_window_size,    9,   0,    64);

        gui_image left, right, depth;
    protected:
        void add_elements(void) override;
    private:
        bool has_resized = false;
    };
}
