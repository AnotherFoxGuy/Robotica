#include "dance/strategies/dance_moves.hpp"
namespace robotica
{
dance_moves& dance_moves::instance()
{
    static dance_moves i;
    return i;
}

void dance_moves::arm_movement_1(int frame)
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
void dance_moves::set_motor_velocity(double l, double r,int mod)
{
    main_window_.left_motor = l *mod;
    main_window_.right_motor = r *mod;
}
void dance_moves::track_movement_1(int frame)
{
    switch (frame)
    {
    case 0:
        set_motor_velocity(0.8, -0.8);
        break;
    case 5:
        set_motor_velocity(0.8, -0.8);
        break;
    case 10:
        set_motor_velocity(0.8, -0.8);
        break;
    case 15:
        set_motor_velocity(-0.8, 0.8);
        break;
    case 20:
        set_motor_velocity(-0.8, 0.8);
        break;
    case 25:
        set_motor_velocity(-0.8, 0.8);
        break;
    case 30:
        set_motor_velocity(0, 0);
        break;
    }
}
void dance_moves::track_movement_2(int frame)
{

    auto n = 0.5;

    switch (frame)
    {
    case 0:
        set_motor_velocity(n, -n);
        break;
    case 5:
        set_motor_velocity(-n, n);
        break;
    case 10:
        set_motor_velocity(n, n);
        break;
    case 15:
        set_motor_velocity(-n, n);
        break;
    case 20:
        set_motor_velocity(n, -n);
        break;
    case 25:
        set_motor_velocity(n, -n);
        break;
    case 30:
        set_motor_velocity(0, 0);
        break;
    }
   
}
void dance_moves::arm_movement_2(int frame)
{
    switch (frame)
    {
    case 0:
        main_window_.arm_long = 1.0;
        break;
    case 5:
        main_window_.arm_short = 0;
        break;
    case 10:
        main_window_.arm_long = 4.0;
        break;
    case 15:
        main_window_.arm_long = 1.0;
        break;
    case 20:
        main_window_.arm_short = 0.0;
        break;
    case 25:
        main_window_.arm_long = 0.0;
        break;
    case 30:
        rotate_arm_base += 2;
        main_window_.arm_base = rotate_arm_base;
        if (rotate_arm_base == 6)
            rotate_arm_base = 0;
        break;
    }
}
void dance_moves::arm_movement_3(int frame)
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
void dance_moves::track_movement_3(int frame)
{
    switch (frame)
    {
    case 0:
        set_motor_velocity(0.2, 0.2);
        break;
    case 5:
        set_motor_velocity(0.5, 0.5);
        break;
    case 10:
        set_motor_velocity(-0.5, -0.5);
        break;
    case 15:
        set_motor_velocity(-0.5, -0.5);
        break;
    case 20:
        set_motor_velocity(-0.9, 0.9);
        break;
    case 25:
        set_motor_velocity(0.9, -0.9);
        break;
    case 30:
        set_motor_velocity(0, 0);
        break;
    }
}

void dance_moves::move_long(int frame, int mod)
{
    switch (frame)
    {
        case 0:
            main_window_.arm_long = 3 * mod;
            break;
        case 5:
            main_window_.arm_long = 3 * mod;
            break;
        case 10:
            main_window_.arm_long = 1.5 * mod;
            break;
        case 15:
            main_window_.arm_long = 1.5 * mod;
            break;
        case 20:
            main_window_.arm_long = 3 * mod;
            break;
        case 25:
            main_window_.arm_long = 3 * mod;
            break;
        case 30:
            main_window_.arm_long = 3 * mod;
            break;
    }
}
void dance_moves::roll_gripper(int frame, int mod)
{
     switch (frame)
    {
    case 0:
        main_window_.gripper_roll = 1.5 * mod;
        break;
    case 5:
        main_window_.gripper_roll = 1.5 * mod;
        break;
    case 10:
        main_window_.gripper_roll = 1.5 * mod;
        break;
    case 15:
        main_window_.gripper_roll = 1.5 * mod;
        break;
    case 20:
        main_window_.gripper_roll = 0;
        break;
    case 25:
        main_window_.gripper_roll = 0;
        break;
    case 30:
        main_window_.gripper_roll = 0;
        break;
    }
}
void dance_moves::rotate_body(int frame, int mod)
{
    auto n = 0.35;
    switch (frame)
    {
    case 0:
        set_motor_velocity(-n, n , mod);
        break;
    case 5:
        set_motor_velocity(-n, n, mod);
        break;
    case 10:
        set_motor_velocity(-n, n, mod);
        break;
    case 15:
        set_motor_velocity(-n, n, mod);
        break;
    case 20:
        set_motor_velocity(-n, n, mod);
        break;
    case 25:
        set_motor_velocity(-n, n, mod);
        break;
    case 30:
        set_motor_velocity(-n, n, mod);
        break;
    }
}

void dance_moves::move_forward(int frame, int mod)
{
    auto n = 0.15;
    switch (frame)
    {
    case 0:
        set_motor_velocity(n, n,mod);
        break;
    case 5:
        set_motor_velocity(n, n, mod);
        break;
    case 10:
        set_motor_velocity(n, n, mod);
        break;
    case 15:
        set_motor_velocity(n, n, mod);
        break;
    case 20:
        set_motor_velocity(n, n, mod);
        break;
    case 25:
        set_motor_velocity(n, n, mod);
        ;
        break;
    case 30:
        set_motor_velocity(n, n, mod);
        break;
    }
}
void dance_moves::rotate_arm(int frame)
{
    switch (frame)
    {
    case 0:
        main_window_.arm_base = 1,5;
        break;
    case 5:
        main_window_.arm_base = 3;
        break;
    case 10:
        main_window_.arm_base = 4,5;
        break;
    case 15:
        main_window_.arm_base = 6;
        break;
    case 20:
        main_window_.arm_base = 6;
        break;
    case 25:
        main_window_.arm_base = 0;
        break;
    case 30:
        main_window_.arm_base = 0;
        break;
    }
}
} // namespace robotica