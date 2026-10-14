#ifndef STAGE_H
#define STAGE_H
#include "world.h"
#include "camera.h"

class Stage {
    public:
        virtual void update(float dt) {};
        virtual void render() {};
        bool go_to_next_stage = false;
};


class PlayStage : public Stage {
    public:

        PlayStage();

        Camera camera;

        void update(float dt);
        void render();

};
#endif