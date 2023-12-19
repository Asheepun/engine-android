#include "engine/engine.h"
#include "engine/3d.h"
#include "engine/renderer2d.h"

Renderer2D_Renderer renderer;
const int WIDTH = 100;
const int HEIGHT = 100;

Vec2f pos = getVec2f(10.0, 10.0);

void Engine_start(){

	Renderer2D_init(&renderer, WIDTH, HEIGHT, Engine_assetManager_p);

}

void Engine_update(){

	pos.x += 0.5;

}

void Engine_draw(){

	glViewport(0, 0, Engine_clientWidth, Engine_clientHeight);

	Vec2f rg = getVec2f(Engine_pointer.pos.x / Engine_clientWidth, Engine_pointer.pos.y / Engine_clientHeight);

	glClearColor(rg.x, 0.0, rg.y, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	Renderer2D_updateDrawSize(&renderer, Engine_clientWidth, Engine_clientHeight);

	Renderer2D_setShader(&renderer, renderer.colorShader);

	Renderer2D_setColor(&renderer, getVec4f(1.0, 1.0, 1.0, 1.0));

	Renderer2D_drawRectangle(&renderer, pos.x, pos.y, 10, 10);


}

void Engine_finnish(){

}
