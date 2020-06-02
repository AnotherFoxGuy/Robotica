#include <vision/world_model.hpp>


namespace robotica {
    world_model& world_model::instance(void) {
        static world_model i { };
        return i;
    }


    void world_model::update(void) {
        // Foreach cached variable, invalidate it.
        expand(
            std::forward_as_tuple(objects, world_mesh, left, right, depth),
            [](auto& cached) { cached.set_valid(false); }
        );
    }


    const std::vector<world_model::world_object>& world_model::get_object_list(void) {
        return objects;
    }


    const mesh& world_model::get_world_mesh(void) {
        return world_mesh;
    }


    std::vector<world_model::world_object> world_model::update_objects(const std::vector<world_model::world_object>& prev) const {
        std::vector<world_object> detections;

        // Get currently detected objects.
        for (auto& [name, classifier] : classifiers) {
            auto subresult = classify(left, classifier);

            std::transform(
                subresult.begin(), 
                subresult.end(), 
                std::back_inserter(detections), 
                [&, name = name](const auto& item) {
                    auto& rbt = robot::instance();

                    const glm::ivec2 center = {
                        item.bounding_rect.x + (item.bounding_rect.width / 2),
                        item.bounding_rect.y + (item.bounding_rect.height / 2)
                    };

                    float size = ((float) depth->at<unsigned char>(center.x, center.y)) * std::sin(
                        ((float) item.bounding_rect.width / robot::instance().get_camera_size(side::LEFT).x) * robot::instance().get_camera_fov(side::LEFT)
                    );


                    // TODO: Calculate with robot position and direction.
                    return world_object {
                        calculate_world_position({ 0, 0 }, 0.0f, 90.0f, center.x, depth->at<unsigned char>(center.x, center.y)),
                        size,
                        (float) item.confidence,
                        name
                    };
                }
            );
        }

        // TODO: Keep old detections & merge them with new ones,
        // remove detections below confidence threshold.
        return detections;
    }


    mesh world_model::update_mesh(void) const {
        // TODO: Get LIDAR mesh here!
        return {};
    }


    cv::Mat world_model::update_depth_map(void) {
        return parallax_depth_map(left, right).filtered;
    }
}