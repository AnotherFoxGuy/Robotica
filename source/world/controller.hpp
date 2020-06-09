#pragma once


namespace robotica {
    class controller {
    public:
        constexpr static int default_timestep = 1000 / 30;  // 30 TPS

        static controller& instance(void);

        controller(int timestep);

        bool update(void);

        int get_timestep(void) const { return timestep; }
        void request_exit(void) { should_exit = true; }
    private:
        int timestep;
        bool should_exit = false;
    };
}