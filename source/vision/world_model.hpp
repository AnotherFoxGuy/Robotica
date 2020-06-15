#pragma once

#include <utility/math.hpp>
#include <utility/cache.hpp>
#include <utility/traits.hpp>
#include <utility/typedefs.hpp>
#include <vision/iclassifier.hpp>
#include <vision/parallax.hpp>
#include <vision/mesher.hpp>
#include <world/robot.hpp>
#include <window/3d/primitive.hpp>

#include <glm/glm.hpp>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <string>
#include <vector>
#include <utility>
#include <type_traits>
#include <tuple>


namespace robotica {
    class world_model {
    public:
        struct world_object {
            glm::vec2 position;
            float size;
            float confidence;
            std::string identity;
        };


        static world_model& instance(void);


        void update(void);

        void add_classifier(unique<iclassifier>&& classifier);
        void remove_classifier(std::string_view name);


        const std::vector<classified_object>& get_raw_object_list(void) const;
        const std::vector<world_object>& get_object_list(void) const;
        const mesh& get_world_mesh(void) const;


        const cv::Mat& get_left_camera (void) const { return left;  }
        const cv::Mat& get_right_camera(void) const { return right; }
        const cv::Mat& get_depth_map   (void) const { return depth; }

        const pointcloud& get_lidar_pointcloud(void) const { return lidar_pointcloud; }
        const mesh& get_lidar_mesh(void) const { return lidar_mesh; }
    private:
        std::vector<classified_object> update_raw_objects(void) const;
        std::vector<world_object> update_objects(const std::vector<world_object>& prev) const;
        mesh update_mesh(void) const;
        cv::Mat update_depth_map(void);


        // Must remain in header because template.
        template <side s> cv::Mat update_camera_data(void) {
            return robot::instance().get_camera_output(s);
        }

        pointcloud update_lidar_pointcloud(void) {
            return robot::instance().get_lidar_pointcloud();
        }

        mesh update_lidar_mesh(void) {
            return create_mesh(lidar_pointcloud);
        }



        // We want to cache these members and bind them to their respective update methods.
        // CTAD and auto are unfortunately not allowed in member declarations, so we have to create some typedefs to do the binding for us.

        // MSVC requires the this-pointer declval to be a const-pointer, GCC requires the opposite.
        // Clang doesn't care either way.
        #ifndef __GNUC__
                template <auto Fn, typename... Args> using bound_t = decltype(std::bind_front(Fn, std::declval<world_model* const>(), std::declval<Args>()...));
        #else
                template <auto Fn, typename... Args> using bound_t = decltype(std::bind_front(Fn, std::declval<world_model*>(), std::declval<Args>()...));
        #endif

        template <auto Fn, typename... Args> using bound_cache = cache<typename make_member_function_info<decltype(Fn)>::return_type, bound_t<Fn, Args...>>;
        #define RBT_WM_CACHE_ITEM(fn, name) bound_cache<&world_model::fn> name { {}, std::bind_front(&world_model::fn, this) }


        RBT_WM_CACHE_ITEM(update_raw_objects,               raw_objects);
        RBT_WM_CACHE_ITEM(update_objects,                   objects);
        RBT_WM_CACHE_ITEM(update_mesh,                      world_mesh);
        RBT_WM_CACHE_ITEM(update_camera_data<side::LEFT>,   left);
        RBT_WM_CACHE_ITEM(update_camera_data<side::RIGHT>,  right);
        RBT_WM_CACHE_ITEM(update_depth_map,                 depth);
        RBT_WM_CACHE_ITEM(update_lidar_pointcloud,          lidar_pointcloud);
        RBT_WM_CACHE_ITEM(update_lidar_mesh,                lidar_mesh);


        // cv::CascadeClassifier is unfortunately not const-correct.
        mutable std::vector<std::pair<std::string, unique<iclassifier>>> classifiers;
    };
}