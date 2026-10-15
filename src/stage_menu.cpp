#include "stage.h"
#include "image.h"
#include "game.h"
#include "input.h"
#include "stage_menu.h"
#include <string>
#include "synth.h"

IntroStage::IntroStage()
{
	selected_option = START;
}

void IntroStage::update(float dt) {

    time_last_button_clicked += dt;

    if (Input::isKeyPressed(SDL_SCANCODE_X) && rendering_tutorial) {
        rendering_tutorial = false;
        return;
    }

    if (Input::isKeyPressed(SDL_SCANCODE_UP) && time_last_button_clicked > 0.3 ) {

        Game::instance->synth.playSample("data/sound/menu.wav", 0.2);
        selected_option = static_cast<OptionSelect>((selected_option + 2 )%3);
        time_last_button_clicked = 0;
    }
    if (Input::isKeyPressed(SDL_SCANCODE_DOWN) && time_last_button_clicked > 0.3) {

        Game::instance->synth.playSample("data/sound/menu.wav", 0.2);
        selected_option = static_cast<OptionSelect>((selected_option + 1) % 3);
        time_last_button_clicked = 0;
    }
    if ((Input::isKeyPressed(SDL_SCANCODE_KP_ENTER) || Input::isKeyPressed(SDL_SCANCODE_Z)) && time_last_button_clicked > 0.3) {
        time_last_button_clicked = 0;

        Game::instance->synth.playSample("data/sound/menu.wav", 0.2);
        if (selected_option == START) {
            Game::instance->current_stage = Game::instance->play_stage;
            if (!Game::instance->music)   
                Game::instance->music = Game::instance->synth.playSample("data/sound/music.wav", 0.5, true);
        }
        if (selected_option == TUTORIAL) {
            rendering_tutorial = true;
        }
        if (selected_option == EXIT) {
            Game::instance->must_exit = true;
        }

    }
}

void IntroStage::render() {  
    if (rendering_tutorial) {  
        Game::instance->framebuffer.drawText("Avoid the ghosts", 10, 10, Game::instance->minifont,4 ,6);  
        Game::instance->framebuffer.drawImage(*Game::instance->images.ghost_image, 10, 17);  


        Game::instance->framebuffer.drawText("Find the door to advance next level", 10, 17+16+1, Game::instance->minifont,4 ,6);
        Game::instance->framebuffer.drawImage(*Game::instance->images.door_image, 10, 17 + 16 + 1 + 6 +1);


        Game::instance->framebuffer.drawText("Move with your arrows", 10, 17 + 16*2 + 1 + 6 + 1 + 6 + 1, Game::instance->minifont,4,6);

        Game::instance->framebuffer.drawText("Select ability with Z to", 10, 17 + 16*2 + 1 + (6 + 1) * 4, Game::instance->minifont, 4, 6);
        Game::instance->framebuffer.drawText("improve your character and start game", 10, 17 + 16*2 + 1 + (6 + 1) * 5, Game::instance->minifont, 4, 6);

        Game::instance->framebuffer.drawText("Exit tutorial with X", 10, 17 + 16*2 + 1 + (6 + 1) * 7, Game::instance->minifont, 4, 6);
    }
    else {

        Image* backgournd_image = Game::instance->images.background;
        Game::instance->framebuffer.drawImage(*backgournd_image, 0, 0);

        Color white = Color(255.0);
        Color red = Color(255.0, 0.0, 0.0);
        Image start_color = (selected_option == START) ? Game::instance->minifont_black : Game::instance->minifont;
        Image tutorial_color = (selected_option == TUTORIAL) ? Game::instance->minifont_black : Game::instance->minifont;;
        Image exit_color = (selected_option == EXIT) ? Game::instance->minifont_black : Game::instance->minifont;;

        Game::instance->framebuffer.drawText("NEVERSCAPE", 10, 10, Game::instance->font);

        Game::instance->framebuffer.drawText("Start", 40, 30, start_color, 4, 6);
        Game::instance->framebuffer.drawText("Tutorial", 40, 60, tutorial_color, 4 ,6);
        Game::instance->framebuffer.drawText("Exit", 40, 80, exit_color, 4, 6);
    }
}

void UpgradeStage::update(float dt) {
    time_last_button_clicked += dt;



    if (Input::isKeyPressed(SDL_SCANCODE_UP) && time_last_button_clicked > 0.3) {
        selected_option = static_cast<UpgradeSelect>((selected_option + 2) % 3);
        time_last_button_clicked = 0;

        Game::instance->synth.playSample("data/sound/menu.wav", 0.2);
    }
    if (Input::isKeyPressed(SDL_SCANCODE_DOWN) && time_last_button_clicked > 0.3) {
        selected_option = static_cast<UpgradeSelect>((selected_option + 1) % 3);
        time_last_button_clicked = 0;
    }
    if ((Input::isKeyPressed(SDL_SCANCODE_KP_ENTER) || Input::isKeyPressed(SDL_SCANCODE_Z)) && time_last_button_clicked > 0.3) {
        time_last_button_clicked = 0;

        Game::instance->synth.playSample("data/sound/power_up.wav", 0.2);
        if (selected_option == TALADRO_SPEED ) {
            Game::instance->world->main_character.taladro_time -= 0.15;
            if (Game::instance->world->main_character.taladro_time <= 0) Game::instance->world->main_character.taladro_time = 0;
        }
        if (selected_option == MORE_BALLS) {
            Game::instance->world->duplicate_balls();
        }
        if (selected_option == BALLS_SPEED) {
            Game::instance->world->main_character.balls_velocity+=0.5;

        }
        Game::instance->current_stage = Game::instance->play_stage;

    }
}


void UpgradeStage::render() {

    Game::instance->framebuffer.drawImage(*Game::instance->images.mage_background,0,0);

    float height_text = Game::instance->framebuffer.height / 4;

    Image taladro = (selected_option == TALADRO_SPEED) ? Game::instance->minifont_black : Game::instance->minifont;
    Image more_balls = (selected_option == MORE_BALLS) ? Game::instance->minifont_black : Game::instance->minifont;
    Image speed = (selected_option == BALLS_SPEED) ? Game::instance->minifont_black : Game::instance->minifont;

    Game::instance->framebuffer.drawText("Increase drill speed", 20, height_text* 1, taladro, 4, 6);
    Game::instance->framebuffer.drawText("Duplicate destruction balls", 20, height_text * 2, more_balls, 4, 6);
    Game::instance->framebuffer.drawText("Increase balls speed", 20, height_text * 3, speed, 4, 6);

}

void EndStage::update(float dt) {
    if (Input::isKeyPressed(SDL_SCANCODE_X)) Game::instance->must_exit = true;


    if (Input::isKeyPressed(SDL_SCANCODE_Z)) {
        delete Game::instance->world;
        Game::instance->world = new World();
        Game::instance->current_stage = Game::instance->intro_stage;
    }

}

void EndStage::render() {
    Image* backgournd_image = Game::instance->images.death_screen;
    Game::instance->framebuffer.drawImage(*backgournd_image,0,0);

    Game::instance->framebuffer.drawText("You Loose", 15, 1, Game::instance->font);

    Game::instance->framebuffer.drawText("Stats:", 20, 2*9 + 6, Game::instance->minifont, 4, 6);

    Game::instance->framebuffer.drawText("You survived:" + std::to_string(Game::instance->world->time_survived) + " seconds", 20, (2*9 + 6) + (6+1)*1, Game::instance->minifont_black, 4, 6);

    Game::instance->framebuffer.drawText("You arrived to level: " + std::to_string(Game::instance->world->level), 20, (2*9 + 6) + (6 + 1) * 2, Game::instance->minifont_black, 4, 6);

    Game::instance->framebuffer.drawText("You had: " + std::to_string(Game::instance->world->balls_list.size()) + " balls", 20, (2*9 + 6) + (6 + 1) * 3, Game::instance->minifont_black, 4, 6);



    Game::instance->framebuffer.drawText("You Will Never Scape", 15, 120-40, Game::instance->font);
}