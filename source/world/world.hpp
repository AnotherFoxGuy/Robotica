#pragma once

#include <world/robot.hpp>

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace robotica
{
struct world_object
{
    std::string name;

    glm::vec2 position;
    float size;
};

class world
{
  public:
    world(int timestep) : objects(), rbt(timestep), timestep(timestep) {}

    void update(void)
    {
        rbt.update();
    }

  private:
    std::vector<world_object> objects;
    robot rbt;

    int timestep;
};
} // namespace robotica