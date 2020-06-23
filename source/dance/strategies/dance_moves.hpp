#pragma once
#include "idance_strategy.hpp"
#include "window/main_window.hpp"
#include "dance/strategies/dance_moves.hpp"
#include "comms/websocket.hpp"
namespace robotica
{
class dance_moves
{
public:
    static dance_moves& instance(void);
    void arm_movement_1(int frame);
    void track_movement_1(int frame);
    void track_movement_2(int frame);
    void arm_movement_2(int frame);
    void arm_movement_3(int frame);
    void track_movement_3(int frame);
    void move_long(int frame ,int mod);
    void roll_gripper(int frame, int mod);
    void rotate_body(int frame, int mod);
    void move_forward(int frame, int mod);
    void rotate_arm(int frame);
    
    void set_motor_velocity(double l, double r,int mod = 0);
  private:
    float rotate_arm_base = 0;
    main_window& main_window_ = main_window::instance();
};
} // namespace robotica
