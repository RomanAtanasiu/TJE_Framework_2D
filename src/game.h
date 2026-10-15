/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "image.h"
#include "utils.h"
#include "synth.h"
#include "stage.h"
#include "world.h"
#include "stage_menu.h"

class Game
{
public:
	static Game* instance;

	//window
	SDL_Window* window;
	SDL_Renderer* renderer;
	int window_width;
	int window_height;

	//some globals
	long frame;
    float time;//total time
	float elapsed_time;//ms from previous frame
	int fps;
	bool must_exit;

	//audio
	Synth synth;
	Synth::SamplePlayback* music = nullptr;


	Stage* current_stage;
	Stage* intro_stage;
	Stage* play_stage;
	Stage* upgrade_stage;
	Stage* end_stage;
	World* world;

	struct{
            Image* wall_image = nullptr;
            Image* player_image = nullptr;
            Image* ghost_image = nullptr;
            Image* block_image = nullptr;
            Image* door_image = nullptr;
			Image* empty_image = nullptr;
			Image* block_big_damage_image = nullptr;  
			Image* block_small_damage_image = nullptr;
			Image* background = nullptr;
			Image* death_screen = nullptr;
			Image* mage_background = nullptr;
	} images;
	Image font;
	Image minifont;
	Image minifont_black;
	Image framebuffer = Image(160, 120);

	

	//ctor
	Game( int window_width, int window_height, SDL_Window* window );

	//my functions

	void load_images();


	//main functions
	void render( void );
	void update( double dt );

	
	void showFramebuffer(Image* img);

	//events
	void onKeyDown( SDL_KeyboardEvent event );
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown( SDL_MouseButtonEvent event );
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onMouseMove(SDL_MouseMotionEvent event);
	void onMouseWheel(SDL_MouseWheelEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
	void onResize(int width, int height);

	//audio stuff
	void enableAudio(); //opens audio channel to play sound
	void onAudio(float* buffer, unsigned int len, double time, SDL_AudioSpec &audio_spec); //called constantly to fill the audio buffer
};


#endif 