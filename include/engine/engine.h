#ifndef ENGINE_H_
#define ENGINE_H_

#include "stdlib.h"
#include "stdbool.h"
#include "engine/geometry.h"
#include "engine/assetManager.h"
#include <vector>

enum Engine_KeyIdentifiers{

	ENGINE_KEY_0,
	ENGINE_KEY_1,
	ENGINE_KEY_2,
	ENGINE_KEY_3,
	ENGINE_KEY_4,
	ENGINE_KEY_5,
	ENGINE_KEY_6,
	ENGINE_KEY_7,
	ENGINE_KEY_8,
	ENGINE_KEY_9,

	ENGINE_KEY_A,
	ENGINE_KEY_B,
	ENGINE_KEY_C,
	ENGINE_KEY_D,
	ENGINE_KEY_E,
	ENGINE_KEY_F,
	ENGINE_KEY_G,
	ENGINE_KEY_H,
	ENGINE_KEY_I,
	ENGINE_KEY_J,
	ENGINE_KEY_K,
	ENGINE_KEY_L,
	ENGINE_KEY_M,
	ENGINE_KEY_N,
	ENGINE_KEY_O,
	ENGINE_KEY_P,
	ENGINE_KEY_Q,
	ENGINE_KEY_R,
	ENGINE_KEY_S,
	ENGINE_KEY_T,
	ENGINE_KEY_U,
	ENGINE_KEY_V,
	ENGINE_KEY_W,
	ENGINE_KEY_X,
	ENGINE_KEY_Y,
	ENGINE_KEY_Z,

	ENGINE_KEY_UP,
	ENGINE_KEY_DOWN,
	ENGINE_KEY_LEFT,
	ENGINE_KEY_RIGHT,

	ENGINE_KEY_SPACE,
	ENGINE_KEY_ESCAPE,

	ENGINE_KEY_SHIFT,
	ENGINE_KEY_CONTROL,

	ENGINE_KEYS_LENGTH,

};

typedef struct Engine_Key{
	unsigned int OSIdentifier;
	char upperCaseString;
	char lowerCaseString;
	bool down;
	bool downed;
	bool upped;
}Engine_Key;

typedef struct Engine_Pointer{
	Vec2f pos;
	Vec2f movement;
	Vec2f lastDownedPos;
	Vec2f lastUppedPos;
	bool down;
	bool downed;
	bool upped;
	int scroll;
}Engine_Pointer;

//GLOBAL VARIABLE DECLARTIONS
extern Engine_Key Engine_keys[ENGINE_KEYS_LENGTH];

extern Engine_Pointer Engine_pointer;

extern std::vector<char> Engine_textInput;

extern int Engine_clientWidth;
extern int Engine_clientHeight;
extern bool Engine_isFullscreen;

extern int Engine_elapsedFrames;
extern float Engine_frameUpdateTime;
extern float Engine_frameDrawTime;
extern float Engine_frameTime;

extern bool Engine_fpsModeOn;

extern AAssetManager *Engine_assetManager_p;

//INTERNAL VARIABLE DECLARTAIONS
extern bool internal_programShouldQuit;

//ENGINE FUNCTIONS

void Engine_start();

void Engine_update();

void Engine_draw();

void Engine_finnish();

void Engine_quit();

//WINDOW FUNCTIONS

void Engine_setFPSMode(bool);

void Engine_setWindowTitle(char *);

void Engine_setWindowSize(int, int);

void Engine_centerWindow();

void Engine_toggleFullscreen();

//INTERNAL FUNCTIONS

void internal_initKeys();
void internal_resetKeys();
void internal_initPointer();
void internal_resetPointer();

#endif
