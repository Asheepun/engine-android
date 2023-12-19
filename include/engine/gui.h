#ifndef GUI_H_
#define GUI_H_

#include "engine/renderer2d.h"
#include "engine/geometry.h"
#include "engine/strings.h"

struct ColorPickerData{
	float angle;
	Vec2f pos;
};

struct TextInputData{
	char text[STRING_SIZE];
	int cursorIndex;
};

extern SmallString GUI_activeTag;
//extern int GUI_activeID;
extern bool GUI_isHovered;
extern bool GUI_textInputIsActive;

void GUI_init();

void GUI_reset();

void GUI_draw(Renderer2D_Renderer *);

void GUI_addTextElement(int, int, const char *, int, Font, Vec4f);

void GUI_addRectElement();

bool GUI_textButton(int, int, const char *, int, Font, bool, const char *);

void GUI_textInput(int, int, int, int, Font, TextInputData *, const char *);

void GUI_colorPicker(int, int, int, ColorPickerData *, const char *, const char *);

void GUI_slider(int, int, int, int, float *, const char *);

Vec4f colorFromColorPickerData(ColorPickerData);

#endif
