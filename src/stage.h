#ifndef STAGE_H
#define STAGE_H
#include "world.h"

class Stage {
    public:
        virtual void update(float dt) {};
        virtual void render() {};

};


class PlayStage : public Stage {
    public:

        PlayStage();

        bool go_to_next_stage = false;
        
        void update(float dt);
        void render();
};
#endif