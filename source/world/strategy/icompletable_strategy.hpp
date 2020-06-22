#pragma once

#include <world/strategy/istrategy.hpp>


namespace robotica {
    class icompletable_strategy : public istrategy {
        virtual bool done(void) = 0;
    };
}