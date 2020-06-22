#pragma once
#include "idance_strategy.hpp"
#include "window/main_window.hpp"

namespace robotica
{

class dance_moves
{
};
class arm_dance_move_1 : public idance_strategy
{
  public:
    static arm_dance_move_1& instance(void);
    void dance(int frame) override;

  private:
    float rotate_arm_base = 0;
    main_window& main_window_ = main_window::instance();
};
class track_dance_move_1 : public idance_strategy
{
  public:
    static track_dance_move_1& instance(void);
    void setMotorVelocity(double l, double r);
    void dance(int frame) override;

  private:
    float rotate_arm_base = 0;
    main_window& main_window_ = main_window::instance();
};
} // namespace robotica
