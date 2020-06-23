#pragma once

#include <world/strategy/istrategy.hpp>


namespace robotica {
    struct icompletable_strategy : public istrategy {
        virtual bool done(void) = 0;
    };
}