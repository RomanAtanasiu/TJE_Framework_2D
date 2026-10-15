#ifndef STAGE_H
#define STAGE_H
#include "world.h"
#include "camera.h"

class Stage {
    public:
        virtual void update(float dt) {};
        virtual void render() {};
        float time_last_button_clicked = 0;
};


class PlayStage : public Stage {
    public:

        PlayStage();

        Camera camera;

        void update(float dt);
        void render();

};
#endif