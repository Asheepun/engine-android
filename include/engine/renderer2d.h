#ifndef RENDERER_2D_H_
#define RENDERER_2D_H_

#include "engine/text.h"
#include "engine/strings.h"
#include "engine/geometry.h"
#include "engine/3d.h"
#include "engine/assetManager.h"

struct Renderer2D_Renderer{
	int width;
	int height;
	Vec2f offset;
	unsigned int rectangleVBO;
	unsigned int rectangleVAO;

	unsigned int textureShader;
	unsigned int colorShader;
	unsigned int textureColorShader;
	unsigned int currentShader;

	bool drawAroundCenter;
};

struct Renderer2D_Texture2D{
	Texture texture;
	int width;
	int height;
};

//INIT FUNCTIONS

void Renderer2D_init(Renderer2D_Renderer *, int, int, AAssetManager *);

void Renderer2D_Texture2D_initFromText(Renderer2D_Texture2D *, const char *, Font font);

//SETTINGS FUNCTIONS

void Renderer2D_updateDrawSize(Renderer2D_Renderer *, int, int);

void Renderer2D_setDrawAroundCenter(Renderer2D_Renderer *, bool);

//DRAWING FUNCTIONS

void Renderer2D_clear(Renderer2D_Renderer *);

void Renderer2D_setShader(Renderer2D_Renderer *, unsigned int);

void Renderer2D_setTexture(Renderer2D_Renderer *, Texture);

void Renderer2D_setColor(Renderer2D_Renderer *, Vec4f);

void Renderer2D_setRotation(Renderer2D_Renderer *, float);

void Renderer2D_drawRectangle(Renderer2D_Renderer *, float, float, float, float);

void Renderer2D_drawText(Renderer2D_Renderer *, const char *, float, float, float, Font);

//void Renderer2D_drawText(Renderer2D_Renderer *, const char *, float, float, int, Font, float);

#endif
