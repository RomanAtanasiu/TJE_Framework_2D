#ifndef STAGE_OTHER_H
#define STAGE_OTHER_H
#include "world.h"
#include "camera.h"
#include "stage.h"
#include "framework.h"


class IntroStage : public Stage {
    public:

        IntroStage();

        enum OptionSelect : uint8 {
            START, TUTORIAL, EXIT
        } selected_option;

        bool rendering_tutorial = false;

        void update(float dt);
        void render();

};



class UpgradeStage : public Stage {
public:

    UpgradeStage() {};

    enum UpgradeSelect : uint8 {
        TALADRO_SPEED , MORE_BALLS, BALLS_SPEED
    } selected_option = TALADRO_SPEED;


    void update(float dt);
    void render();

};


class EndStage : public Stage {
public:
    EndStage() {}

    void update(float dt);
    void render();

};

#endif