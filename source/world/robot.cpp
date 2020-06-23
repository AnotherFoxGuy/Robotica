#include <world/robot.hpp>
#include <world/controller.hpp>
#include <comms/websocket.hpp>
#include <vision/world_model.hpp>
#include <vision/heat_measure.hpp>

#include <magic_enum.hpp>

#include <string_view>
#include <algorithm>


namespace robotica {
    robot& robot::instance(void) {
        static robot i { controller::instance().get_timestep() };
        return i;
    }

    robot::robot(int timestep) :
        rbt(new webots::Robot()),
        left_camera(rbt->getCamera(camera_names[0])),
        right_camera(rbt->getCamera(camera_names[1])),
        left_motor(rbt->getMotor(motor_names[0])),
        right_motor(rbt->getMotor(motor_names[1])),
        arm_base(rbt->getMotor(motor_names[2])),
        arm_short(rbt->getMotor(motor_names[3])),
        arm_long(rbt->getMotor(motor_names[4])),
        gripper_left(rbt->getMotor(motor_names[5])),
        gripper_right(rbt->getMotor(motor_names[6])),
        gripper_roll(rbt->getMotor(motor_names[7])),
        gripper_pitch(rbt->getMotor(motor_names[8])),
        dist_left(rbt->getDistanceSensor(distance_sensor_names[0])),
        dist_right(rbt->getDistanceSensor(distance_sensor_names[1])),
        compass(rbt->getCompass(compass_name)),
        lidar(rbt->getLidar(lidar_name)),
        scale(rbt->getTouchSensor(scale_name)),
        timestep(timestep),
        eye_distance(0.03f),
        eye_height(0.028f)
    {
        left_camera->enable(timestep);
        right_camera->enable(timestep);

        left_motor->setPosition(INFINITY);
        right_motor->setPosition(INFINITY);

        left_motor->setVelocity(0);
        right_motor->setVelocity(0);

        speaker = rbt->getSpeaker(speaker_name);

        speaker->setEngine("microsoft");
        speaker->setLanguage("de-DE");

        display = rbt->getDisplay(display_name);

        led_left = rbt->getLED(led_names[0]);
        led_right = rbt->getLED(led_names[1]);

        //emotes = display->imageLoad("emoticons.png");

        //provide samplingPeriod in milliseconds
        compass->enable(100);
        lidar->enable(100);
        scale->enable(100);
        dist_left->enable(100);
        dist_right->enable(100);

        compass->enable(timestep);

        lidar->enable(timestep);
        lidar->enablePointCloud();

        // Make sure to disconnect from webots on quick exit as well.
        std::at_quick_exit([]() {
            auto& self = robot::instance();

            delete self.rbt;
            self.manually_destroyed = true;
        });
    }


    robot::~robot(void) {
        if (!manually_destroyed) delete rbt;
    }

    double robot::get_distance(side side) const {
        return (side == side::LEFT) ? dist_left->getValue() : dist_right->getValue();
    }

    double robot::get_bearing_in_degrees() {
        const double *north = compass->getValues();
        double rad = atan2(north[0], north[2]);
        double bearing = (rad - 1.5708) / M_PI * 180.0;
        if (bearing < 0.0)
            bearing = bearing + 360.0;
        return bearing;
    }

    double robot::get_bearing_in_radian() {
        const double *north = compass->getValues();
        double bearing = atan2(north[0], north[2]);
        if (bearing < 0.0)
            bearing = bearing + 6.28319;
        return bearing;
    }


    bool robot::update(void) {
        auto& window = main_window::instance();

        const std::array components {
            std::tuple { arm_base,      &window.arm_base,      (float)  1 },
            std::tuple { arm_short,     &window.arm_short,     (float)  1 },
            std::tuple { arm_long,      &window.arm_long,      (float)  1 },
            std::tuple { gripper_left,  &window.gripper,       (float)  1 },
            std::tuple { gripper_right, &window.gripper,       (float) -1 },
            std::tuple { gripper_roll,  &window.gripper_roll,  (float)  1 },
            std::tuple { gripper_pitch, &window.gripper_pitch, (float)  1 }
        };

        int result = rbt->step(timestep);
        if (result != -1)
        {
            for (auto& [component, value, factor] : components) (*component).setPosition(factor * (**value));

            (*left_motor).setVelocity(-(window.left_motor * window.speed * 0.006));   //(max) 6 m/s ~ 21 km/h
            (*right_motor).setVelocity(-(window.right_motor * window.speed * 0.006)); //(max) 6 m/s ~ 21 km/h

            // 0.01 = 0.0373 / 3.73 => default force on the scale / gravity of the "moon" (its mars gravity)
            websocket::instance().sendData("weight", (scale->getValue() / 3.73) - 0.01);
            websocket::instance().sendData("compass", get_bearing_in_degrees());
            std::stringstream worst;
            const char* separator = "";
            worst << "{\"Temps\": [";
            for (auto& detection : world_model::instance().get_raw_object_list())
            {
                // Show temperature. Ignore far away pools for accuracy.
                if (detection.type == "Pool" && detection.bounding_rect.height > 4)
                {
                    auto avg_color = std::any_cast<cv::Vec3b>(detection.data);
                    float temp = calculate_temperature(avg_color);
                    temperature tempclass = temperature_class(temp);
                    worst << separator << "\"" << magic_enum::enum_name(tempclass) << " (" << temp << ")"
                          << "\"";
                    separator = ",";
                }
            }
            worst << "] }";
            websocket::instance().sendData(worst.str());

            // Emotions
            update_emotion();
        }
        return (result != -1);
    }


    void robot::update_emotion()
    {
        if (displayed_emotion != current_emotion) {
            std::cout << "loading emotion" << std::endl;
            auto current = fs::current_path();
            auto z = display->imageLoad(current.parent_path().parent_path().string() + "\\images\\" + current_emotion + ".jpg");
            displayed_emotion = current_emotion;
            //display->setColor(0xFF00FF);
            display->setAlpha(1);
            display->setOpacity(1);
            display->imagePaste(z, 0, 0, false);
            //display->fillRectangle(0, 0, 400, 400);
        }
    }

    cv::Mat robot::get_camera_output(side side) const {
        auto& camera = (side == side::LEFT) ? left_camera : right_camera;

        cv::Mat image = cv::Mat(cv::Size(camera->getWidth(), camera->getHeight()), CV_8UC4);
        image.data = (uchar*) camera->getImage();

        // WeBots will sometimes not send any data because its stupid.
        if (image.empty()) return cv::imread(ROOT_DIR "/assets/debug_image.png");

        cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);

        return image;
    }

    void robot::set_led_color(int left, int right){
        led_left->set(left);
        led_right->set(right);
    }

    float robot::get_camera_fov(side side) const {
        auto& camera = (side == side::LEFT) ? left_camera : right_camera;
        return camera->getFov();
    }


    glm::ivec2 robot::get_camera_size(side side) const {
        auto& camera = (side == side::LEFT) ? left_camera : right_camera;
        return { camera->getWidth(), camera->getHeight() };
    }


    float robot::get_camera_focal_length(side side) const {
        auto& camera = (side == side::LEFT) ? left_camera : right_camera;
        return camera->getFocalDistance();
    }


    float robot::get_camera_baseline(void) const {
        return 0.065;    // Hardcoded value from PROTO file. Supervisor mode could also obtain this.
    }


    pointcloud robot::get_lidar_pointcloud(void) const {
        auto& settings = main_window::instance();
        std::size_t points = lidar->getNumberOfPoints();

        pointcloud pc;
        pc.reserve(points);

        // We discard some points here, so the data is easier to work with for the pathfinding algorithm.
        // Normally we'd just change the sensor, but WeBots doesn't allow us to set the number of points per scanline below some arbitrary limit.
        for (std::size_t i = 0; i < points; ++i) {
            int keep_every_nth_point = settings.lidar_inverse_discard_ratio - std::clamp<float>(
                (float(i) / float(points)) * (settings.lidar_inverse_discard_ratio - 1) * settings.lidar_nearby_suppression, 
                0, 
                settings.lidar_inverse_discard_ratio - 1
            );


                const auto& pt = lidar->getPointCloud()[i];
                pc.push_back({ glm::vec3{ pt.x * settings.lidar_scale_factor, pt.y * settings.lidar_scale_factor, pt.z * settings.lidar_scale_factor } });

        }

        return pc;
    }
}
