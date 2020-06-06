#include <vision/world_model.hpp>
#include <window/main_window.hpp>
#include <utility/utility.hpp>


namespace robotica {
    world_model& world_model::instance(void) {
        static world_model i { };
        return i;
    }


    void world_model::update(void) {
        // Foreach cached variable, invalidate it.
        expand(
            std::forward_as_tuple(raw_objects, objects, world_mesh, left, right, depth),
            [](auto& cached) { cached.set_valid(false); }
        );
    }


    void world_model::add_classifier(std::string_view mapname, std::string_view filename) {
        cv::CascadeClassifier cc { (classifier_folder / filename).string() };
        classifiers.push_back({ std::string(mapname), cc });
    }


    void world_model::remove_classifier(std::string_view mapname) {
        auto it = std::find_if(classifiers.begin(), classifiers.end(), [&](const auto& pair) { return pair.first == mapname; });
        if (it != classifiers.end()) classifiers.erase(it);
    }


    const std::vector<detected_object> world_model::get_raw_object_list(void) const {
        return raw_objects;
    }


    const std::vector<world_model::world_object>& world_model::get_object_list(void) const {
        return objects;
    }


    const mesh& world_model::get_world_mesh(void) const {
        return world_mesh;
    }


    std::vector<detected_object> world_model::update_raw_objects(void) const {
        std::vector<detected_object> result;

        for (auto& [name, classifier] : classifiers) {
            auto subresult = classify(left, classifier, name);
            remove_if(subresult, [](const auto& e) { return e.confidence < main_window::instance().min_confidence; });

            std::move(subresult.begin(), subresult.end(), std::back_inserter(result));
        }

        return result;
    }


    std::vector<world_model::world_object> world_model::update_objects(const std::vector<world_model::world_object>& prev) const {
        std::vector<world_object> detections;
        std::vector<float> measurements;


        std::transform(
            (*raw_objects).begin(), 
            (*raw_objects).end(), 
            std::back_inserter(detections), 
            [&](const auto& item) {
                auto& rbt = robot::instance();

                const glm::ivec2 center = {
                    item.bounding_rect.x + (item.bounding_rect.width / 2),
                    item.bounding_rect.y + (item.bounding_rect.height / 2)
                };


                // Take a bunch of measurements and use the median.
                measurements.clear();
                for (int x = item.bounding_rect.x; x < item.bounding_rect.x + item.bounding_rect.width; x += 4) {
                    for (int y = item.bounding_rect.y; y < item.bounding_rect.y + item.bounding_rect.height; y += 4) {
                        uchar d = 255 - depth->at<uchar>(y, x);
                        measurements.push_back((float) d);
                    }
                }

                std::sort(measurements.begin(), measurements.end());
                float distance = (measurements.size() > 0) ? measurements[measurements.size() / 2] : 0;

                float size = distance * std::sin(
                    ((float) item.bounding_rect.width / rbt.get_camera_size(side::LEFT).x) * rbt.get_camera_fov(side::LEFT)
                );


                // TODO: Calculate with robot position and direction.
                // Or possibly just use robot as origin?
                return world_object {
                    calculate_world_position(glm::vec2{ 0, 0 }, 0.0f, rbt.get_camera_fov(side::LEFT), center.x, distance),
                    size,
                    (float) item.confidence,
                    item.type
                };
            }
        );

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