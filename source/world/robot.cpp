#include <world/robot.hpp>
#include <world/controller.hpp>


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
        compass(rbt->getCompass(compass_name)),
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

        compass->enable(100);
    }

    double robot::get_bearing_in_degrees() {
        const double *north = compass->getValues();
        double rad = atan2(north[0], north[1]);
        double bearing = (rad - 1.5708) / M_PI * 180.0;
        if (bearing < 0.0)
            bearing = bearing + 360.0;
        return bearing;
    }


    bool robot::update(void) {
        auto& window = main_window::instance();


        std::cout << get_bearing_in_degrees() << std::endl;
        int result;
        
        if (result = rbt->step(timestep); result != -1) {
            // Go in circles.
            (*left_motor).setVelocity(window.left_motor * max_speed);
            (*right_motor).setVelocity(window.right_motor * max_speed);
        }

        return (result != -1);
    }


    cv::Mat robot::get_camera_output(side side) const {
        auto& camera = (side == side::LEFT) ? left_camera : right_camera;

        cv::Mat image = cv::Mat(cv::Size(camera->getWidth(), camera->getHeight()), CV_8UC4);
        image.data = (uchar*) camera->getImage();

        cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);

        return image;
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
        return 0.03;    // Hardcoded value from PROTO file. Supervisor mode could also obtain this.
    }
}