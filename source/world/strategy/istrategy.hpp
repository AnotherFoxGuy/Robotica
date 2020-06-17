#pragma once


namespace robotica {
    struct istrategy {
        virtual void init(void) {}
        virtual void exit(void) {}
        virtual void loop(void) {}
    };
}