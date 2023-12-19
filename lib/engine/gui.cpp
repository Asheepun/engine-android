#include "engine/engine.h"

#include "math.h"

#include "engine/gui.h"

enum ElementType{
	ELEMENT_TYPE_RECT,
	ELEMENT_TYPE_TEXT,
	ELEMENT_TYPE_COLOR_WHEEL,
	ELEMENT_TYPE_COLOR_TRIANGLE,
};

struct Element{
	enum ElementType type;
	int x;
	int y;
	int width;
	int height;
	unsigned int shader;
	char textureName[STRING_SIZE];
	Vec4f color;
};

Shader colorWheelShader;
Shader colorTriangleShader;

std::vector<Renderer2D_Texture2D> cachedTextTextures;
std::vector<Element> elements;

int WIDTH = 1920;
int HEIGHT = 1080;

//int GUI_activeID = -1;
SmallString GUI_activeTag;
bool GUI_isHovered = false;
bool GUI_textInputIsActive = false;

void GUI_init(){

	Shader_init(&colorWheelShader, "color-wheel-shader", "shaders/renderer2d/color-vertex-shader.glsl", "shaders/color-wheel-fragment-shader.glsl");
	Shader_init(&colorTriangleShader, "color-triangle-shader", "shaders/renderer2d/color-vertex-shader.glsl", "shaders/color-triangle-fragment-shader.glsl");

}

void GUI_reset(){

	elements.clear();

	GUI_isHovered = false;

}

void GUI_draw(Renderer2D_Renderer *renderer_p){

	glDisable(GL_DEPTH_TEST);

	for(int i = 0; i < elements.size(); i++){

		Element *element_p = &elements[i];
		
		if(element_p->type == ELEMENT_TYPE_RECT){

			Renderer2D_setShader(renderer_p, renderer_p->colorShader);

			Renderer2D_setColor(renderer_p, element_p->color);
			Renderer2D_drawRectangle(renderer_p, element_p->x, element_p->y, element_p->width, element_p->height);

		}
		
		if(element_p->type == ELEMENT_TYPE_TEXT){

			Renderer2D_Texture2D *texture_p = NULL;
			for(int j = 0; j < cachedTextTextures.size(); j++){
				if(strcmp(cachedTextTextures[j].texture.name, element_p->textureName) == 0){
					texture_p = &cachedTextTextures[j];
					break;
				}
			}

			Renderer2D_setShader(renderer_p, renderer_p->textureColorShader);

			Renderer2D_setColor(renderer_p, element_p->color);
			Renderer2D_setTexture(renderer_p, texture_p->texture);
			Renderer2D_drawRectangle(renderer_p, element_p->x, element_p->y, element_p->width, element_p->height);

		}

		if(element_p->type == ELEMENT_TYPE_COLOR_WHEEL){

			Renderer2D_setShader(renderer_p, colorWheelShader.ID);

			GL3D_uniformFloat(colorWheelShader.ID, "inputAngle", element_p->color.x);

			Renderer2D_drawRectangle(renderer_p, element_p->x, element_p->y, element_p->width, element_p->height);

			//Renderer2D_setShader(renderer_p, colorTriangleShader.ID);

			//GL3D_uniformVec2f(colorTriangleShader.ID, "inputPos", getVec2f(element_p->color.y, element_p->color.z));

			//Renderer2D_drawRectangle(renderer_p, element_p->x + element_p->width * 0.3, element_p->y + element_p->height * 0.25, element_p->width * 0.4, element_p->height * 0.4);
		
		}
		
		if(element_p->type == ELEMENT_TYPE_COLOR_TRIANGLE){

			Renderer2D_setShader(renderer_p, colorTriangleShader.ID);

			GL3D_uniformFloat(colorWheelShader.ID, "inputAngle", element_p->color.x);

			GL3D_uniformVec2f(colorTriangleShader.ID, "inputPos", getVec2f(element_p->color.y, element_p->color.z));

			Renderer2D_drawRectangle(renderer_p, element_p->x, element_p->y, element_p->width, element_p->height);
		
		}

	}

	glEnable(GL_DEPTH_TEST);

}

void GUI_addTextElement(int x, int y, const char *text, int fontSize, Font font, Vec4f color){

	Renderer2D_Texture2D texture;

	bool foundTexture = false;
	for(int i = 0; i < cachedTextTextures.size(); i++){
		if(strcmp(text, cachedTextTextures[i].texture.name) == 0){
			texture = cachedTextTextures[i];
			foundTexture = true;
			break;
		}
	}

	if(!foundTexture){
		Renderer2D_Texture2D_initFromText(&texture, text, font);
		cachedTextTextures.push_back(texture);
	}

	int width = texture.width * ((float)fontSize / (float)texture.height);
	int height = fontSize;

	{
		Element element;
		element.type = ELEMENT_TYPE_TEXT;
		element.x = x;
		element.y = y;
		element.width = width;
		element.height = height;
		element.color = color;
		String_set(element.textureName, texture.texture.name, STRING_SIZE);
		elements.push_back(element);
	}
	
}

void GUI_addRectElement(int x, int y, int width, int height, Vec4f color){

	Element element;

	element.type = ELEMENT_TYPE_RECT;
	element.x = x;
	element.y = y;
	element.width = width;
	element.height = height;
	element.color = color;

	elements.push_back(element);

}

bool GUI_textButton(int x, int y, const char *text, int fontSize, Font font, bool active, const char *tag){

	bool returnValue = false;

	Vec2f pointerPos = Engine_pointer.pos * ((float)WIDTH / (float)Engine_clientWidth);

	int borderSize = fontSize / 10;
	int margin = fontSize / 10;
	float alpha = 1.0;

	if(active){
		alpha -= 0.3;
	}

	Renderer2D_Texture2D texture;

	bool foundTexture = false;
	for(int i = 0; i < cachedTextTextures.size(); i++){
		if(strcmp(text, cachedTextTextures[i].texture.name) == 0){
			texture = cachedTextTextures[i];
			foundTexture = true;
			break;
		}
	}

	if(!foundTexture){
		Renderer2D_Texture2D_initFromText(&texture, text, font);
		cachedTextTextures.push_back(texture);
	}

	int width = texture.width * ((float)fontSize / (float)texture.height);
	int height = fontSize;

	bool hovered = false;
	if(pointerPos.x > x && pointerPos.x < x + width + borderSize * 2 + margin * 2
	&& pointerPos.y > y && pointerPos.y < y + height + borderSize * 2 + margin * 2){
		alpha -= 0.5;
		hovered = true;
		GUI_isHovered = true;
	}

	if(Engine_pointer.upped
	&& hovered
	&& strcmp(GUI_activeTag, tag) == 0){
		returnValue = true;
		String_set(GUI_activeTag, "", SMALL_STRING_SIZE);
	}

	if(Engine_pointer.downed){
		if(strcmp(GUI_activeTag, tag) == 0){
			String_set(GUI_activeTag, "", SMALL_STRING_SIZE);
		}
		if(hovered){
			String_set(GUI_activeTag, tag, SMALL_STRING_SIZE);
			GUI_textInputIsActive = false;
		}
	}

	GUI_addRectElement(x, y, width + margin * 2 + borderSize * 2, height + margin * 2 + borderSize * 2, getVec4f(0.3, 0.3, 0.3, alpha));

	GUI_addRectElement(x + borderSize, y + borderSize, width + margin * 2, height + margin * 2, getVec4f(0.7, 0.7, 0.7, alpha));

	{
		Element element;
		element.type = ELEMENT_TYPE_TEXT;
		element.x = x + margin + borderSize;
		element.y = y + margin + borderSize;
		element.width = width;
		element.height = height;
		element.color = getVec4f(0.3, 0.3, 0.3, alpha);
		String_set(element.textureName, texture.texture.name, STRING_SIZE);
		elements.push_back(element);
	}

	return returnValue;

}

void GUI_textInput(int x, int y, int width, int fontSize, Font font, TextInputData *textInputData_p, const char *tag){

	Vec2f pointerPos = Engine_pointer.pos * ((float)WIDTH / (float)Engine_clientWidth);

	float alpha = 1.0;

	if(strcmp(GUI_activeTag, tag) == 0){

		alpha -= 0.3;
		
		for(int i = 0; i < Engine_textInput.size(); i++){

			if(Engine_textInput[i] == 8){//backspace
				if(strlen(textInputData_p->text) > 0){
					textInputData_p->text[strlen(textInputData_p->text) - 1] = *"\0";
				}
			}else{
				String_append_char(textInputData_p->text, Engine_textInput[i]);
			}

		}

	}

	bool hovered = false;

	if(pointerPos.x > x && pointerPos.x < x + width
	&& pointerPos.y > y && pointerPos.y < y + fontSize){
		hovered = true;
		GUI_isHovered = true;
	}

	if(hovered){
		alpha -= 0.5;

		if(Engine_pointer.downed){
			String_set(GUI_activeTag, tag, SMALL_STRING_SIZE);
			GUI_textInputIsActive = true;
		}

	}else{
		if(Engine_pointer.downed
		&& strcmp(GUI_activeTag, tag) == 0){
			String_set(GUI_activeTag, "", SMALL_STRING_SIZE);
			GUI_textInputIsActive = false;
		}
	}

	GUI_addRectElement(x, y, width, fontSize, getVec4f(0.7, 0.7, 0.7, alpha));
	
	GUI_addTextElement(x, y, textInputData_p->text, fontSize, font, getVec4f(0.3, 0.3, 0.3, alpha));

}

void GUI_colorPicker(int x, int y, int size, ColorPickerData *colorPickerData_p, const char *wheelTag, const char *triangleTag){

	Vec2f pointerPos = Engine_pointer.pos * ((float)WIDTH / (float)Engine_clientWidth);

	Vec2f center = getVec2f(x + size / 2, y + size / 2);

	Vec2f relativePos = (pointerPos - center) / size;

	//handle wheel
	bool hovered = false;

	if(length(relativePos) < 0.4
	&& length(relativePos) > 0.3){
		hovered = true;
		GUI_isHovered = true;
	}

	float angle = -atan2(relativePos.y, relativePos.x);
	if(angle < 0.0){
		angle = M_PI * 2.0 + angle;
	}

	if(Engine_pointer.downed
	&& hovered){
		String_set(GUI_activeTag, wheelTag, SMALL_STRING_SIZE);
	}

	if(Engine_pointer.upped
	&& strcmp(GUI_activeTag, wheelTag) == 0){
		String_set(GUI_activeTag, "", SMALL_STRING_SIZE);
		GUI_textInputIsActive = false;
	}

	if(strcmp(GUI_activeTag, wheelTag) == 0){
		colorPickerData_p->angle = angle;
	}

	{
		Element element;
		element.type = ELEMENT_TYPE_COLOR_WHEEL;
		element.x = x;
		element.y = y;
		element.width = size;
		element.height = size;

		element.color.x = colorPickerData_p->angle;

		elements.push_back(element);
	}

	//handle triangle
	x = x + size * 0.30;
	y = y + size * 0.25;
	size *= 0.4;
	center = getVec2f(x + size * 0.5, y + size * 0.5);
	relativePos = (pointerPos - center) / size;
	relativePos.y += 0.5;
	relativePos.x *= 2.0;

	hovered = false;

	if(relativePos.y >= 0.0 && relativePos.y <= 1.0
	&& fabs(relativePos.x) + 1.0 - relativePos.y <= 1.0
	|| length(relativePos - colorPickerData_p->pos) < 0.09){
		hovered = true;
		GUI_isHovered = true;
	}

	if(hovered){
		if(Engine_pointer.downed){
			String_set(GUI_activeTag, triangleTag, SMALL_STRING_SIZE);
			GUI_textInputIsActive = false;
		}
	}

	if(strcmp(GUI_activeTag, triangleTag) == 0){

		colorPickerData_p->pos = relativePos;

		if(colorPickerData_p->pos.y > 1.0){
			colorPickerData_p->pos.y = 1.0;
		}
		if(colorPickerData_p->pos.y < 0.0){
			colorPickerData_p->pos.y = 0.0;
		}
		if(colorPickerData_p->pos.x > colorPickerData_p->pos.y){
			colorPickerData_p->pos.x = colorPickerData_p->pos.y;
		}
		if(colorPickerData_p->pos.x < -colorPickerData_p->pos.y){
			colorPickerData_p->pos.x = -colorPickerData_p->pos.y;
		}

	}

	if(Engine_pointer.upped
	&& strcmp(GUI_activeTag, triangleTag) == 0){
		String_set(GUI_activeTag, "", SMALL_STRING_SIZE);
	}

	{
		Element element;
		element.type = ELEMENT_TYPE_COLOR_TRIANGLE;
		element.x = x;
		element.y = y;
		element.width = size;
		element.height = size;

		element.color.x = colorPickerData_p->angle;
		element.color.y = colorPickerData_p->pos.x;
		element.color.z = colorPickerData_p->pos.y;

		elements.push_back(element);
	}

}

float SIXTH = M_PI * 2.0 / 6.0;

Vec4f colorFromColorPickerData(ColorPickerData colorPickerData){

	float angle = colorPickerData.angle;
	Vec2f pos = colorPickerData.pos;

	Vec3f rgb = getVec3f(0.0, 0.0, 0.0);

	rgb.x = (float)(angle < SIXTH) * angle / SIXTH +
			(float)(angle >= SIXTH && angle < SIXTH * 3) +
			(float)(angle >= SIXTH * 3 && angle < SIXTH * 4) * (SIXTH * 4 - angle) / SIXTH;

	rgb.y = (float)(angle >= SIXTH * 2 && angle < SIXTH * 3) * (angle - SIXTH * 2) / SIXTH +
			(float)(angle >= SIXTH * 3 && angle < SIXTH * 5) +
			(float)(angle >= SIXTH * 5 && angle < SIXTH * 6) * (SIXTH * 6 - angle) / SIXTH;

	rgb.z = (float)(angle >= SIXTH * 4 && angle < SIXTH * 5) * (angle - SIXTH * 4) / SIXTH +
			(float)(angle >= SIXTH * 5 && angle < SIXTH * 6 || angle >= 0 && angle < SIXTH) +
			(float)(angle >= SIXTH && angle < SIXTH * 2) * (SIXTH * 2 - angle) / SIXTH;

	float blackFactor = (pos.y - pos.x) / 2.0;
	float whiteFactor = (pos.x + pos.y) / 2.0;

	rgb = getVec3f(0.0, 0.0, 0.0) * blackFactor + getVec3f(1.0, 1.0, 1.0) * whiteFactor + rgb * (1.0 - blackFactor - whiteFactor);


	return getVec4f(rgb.x, rgb.y, rgb.z, 1.0);

}

void GUI_slider(int x, int y, int width, int height, float *factor_p, const char *tag){

	bool hovered = false;
	float alpha = 1.0;

	Vec2f pointerPos = Engine_pointer.pos * ((float)WIDTH / (float)Engine_clientWidth);

	if(pointerPos.x > x && pointerPos.x < x + width
	&& pointerPos.y > y && pointerPos.y < y + height){
		hovered = true;
		GUI_isHovered = true;
	}

	if(hovered){

		alpha -= 0.5;

		if(Engine_pointer.downed){
			String_set(GUI_activeTag, tag, SMALL_STRING_SIZE);
			GUI_textInputIsActive = false;
		}

	}

	if(Engine_pointer.upped
	&& strcmp(GUI_activeTag, tag) == 0){
		String_set(GUI_activeTag, "", SMALL_STRING_SIZE);
	}

	if(strcmp(GUI_activeTag, tag) == 0){
		*factor_p = fmin(1.0, fmax(0.0, (float)(pointerPos.x - x) / (float)width));
	}

	GUI_addRectElement(x, y + (float)height * 0.4, width, (float)height * 0.2, getVec4f(0.7, 0.7, 0.7, alpha));

	int knobWidth = width * 0.1;
	int knobX = x + *factor_p * (float)(width - knobWidth);

	GUI_addRectElement(knobX, y, knobWidth, height, getVec4f(0.3, 0.3, 0.3, alpha));

}
