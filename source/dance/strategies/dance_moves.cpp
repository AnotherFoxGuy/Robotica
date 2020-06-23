#include "dance/strategies/dance_moves.hpp"
namespace robotica
{
arm_dance_move_1& arm_dance_move_1::instance()
{
    static arm_dance_move_1 i;
    return i;
}
void arm_dance_move_1::dance(int frame)
{
    switch (frame)
    {
    case 0:
        main_window_.arm_long = 3.12;
        break;
    case 5:
        main_window_.arm_short = -1.56;
        break;
    case 10:
        main_window_.arm_long = 5.0;
        break;
    case 15:
        main_window_.arm_long = 4.0;
        break;
    case 20:
        main_window_.arm_long = 5.0;
        break;
    case 25:
        main_window_.arm_long = 4.0;
        break;
    case 30:
        rotate_arm_base += 1.5;
        main_window_.arm_base = rotate_arm_base;
        if (rotate_arm_base == 6)
            rotate_arm_base = 0;
        break;
    }
}
track_dance_move_1& track_dance_move_1::instance()
{
    static track_dance_move_1 i;
    return i;
}
void track_dance_move_1::dance(int frame)
{
    switch (frame)
    {
    case 0:
        setMotorVelocity(0.8, -0.8);
        break;
    case 5:
        setMotorVelocity(0.8, -0.8);
        break;
    case 10:
        setMotorVelocity(0.8, -0.8);
        break;
    case 15:
        setMotorVelocity(-0.8, 0.8);
        break;
    case 20:
        setMotorVelocity(-0.8, 0.8);
        break;
    case 25:
        setMotorVelocity(-0.8, 0.8);
        break;
    case 30:
        setMotorVelocity(0, 0);
        break;
    }
}
void track_dance_move_1::setMotorVelocity(double l, double r)
{
    main_window_.left_motor = l;
    main_window_.right_motor = r;
}

arm_dance_move_3& arm_dance_move_3::instance()
{
    static arm_dance_move_3 i;
    return i;
}
void arm_dance_move_3::dance(int frame)
{
    switch (frame)
    {
    case 0:
        main_window_.arm_long = 1.5;
        main_window_.arm_short = -1.5;
        break;
    case 5:
        main_window_.arm_long = 4.6;
        main_window_.gripper_pitch = -1.5;
        main_window_.gripper_roll = -3.142;
        break;
    case 10:
        main_window_.arm_base = 6;
        main_window_.gripper_pitch = 1.5;
        break;
    case 15:
        main_window_.gripper_pitch = -1.5;
        break;
    case 20:
        main_window_.gripper_pitch = 0;
        main_window_.gripper_roll = 3.142;
        break;
    case 25:
        main_window_.arm_base = 0;
        main_window_.arm_long = 3;
        break;
    case 30:
        main_window_.arm_short = 1.5;
        main_window_.arm_short = -1.5;
        main_window_.gripper_roll = 0;
        break;
    }
}
track_dance_move_3& track_dance_move_3::instance()
{
    static track_dance_move_3 i;
    return i;
}
void track_dance_move_3::setMotorVelocity(double l, double r)
{
    main_window_.left_motor = l;
    main_window_.right_motor = r;
}
void track_dance_move_3::dance(int frame)
{
    switch (frame)
    {
    case 0:
        setMotorVelocity(0.5, 0.5);
        break;
    case 5:
        setMotorVelocity(0.5 ,0.5);
        break;
    case 10:
        setMotorVelocity(-0.5, -0.5);
        break;
    case 15:
        setMotorVelocity(-0.5, -0.5);
        break;
    case 20:
        setMotorVelocity(-0.9, 0.9);
        break;
    case 25:
        setMotorVelocity(0.9, -0.9);
        break;
    case 30:
        setMotorVelocity(0, 0);
        break;
    }
}

} // namespace robotica