#include "engine/text.h"
#include "engine/renderer2d.h"
#include "engine/shaders.h"
#include "engine/3d.h"
#include "engine/files.h"
#include "engine/shaders.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "stdio.h"
#include "math.h"
#include "string.h"

//INIT FUNCTIONS

void Renderer2D_init(Renderer2D_Renderer *renderer_p, int width, int height, AAssetManager *assetManager_p){

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	renderer_p->width = width;
	renderer_p->height = height;

	renderer_p->drawAroundCenter = false;

	static float rectangleVertices[] = {

		1, 1, 		1, 0,
		1, -1, 		1, 1,
		-1, -1, 	0, 1,

		1, 1, 		1, 0,
		-1, -1, 	0, 1,
		-1, 1, 		0, 0,

	};

	glGenBuffers(1, &renderer_p->rectangleVBO);

	glBindBuffer(GL_ARRAY_BUFFER, renderer_p->rectangleVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &renderer_p->rectangleVAO);
	glBindVertexArray(renderer_p->rectangleVAO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Font font = getFont("assets/fonts/times.ttf", 100);

	//Texture_initFromText(&renderer_p->textTexture, "", font);

	//init color shader
	{
		unsigned int vertexShader = getCompiledShader("shaders/renderer2d/color-vertex-shader.glsl", GL_VERTEX_SHADER, assetManager_p);
		unsigned int fragmentShader = getCompiledShader("shaders/renderer2d/color-fragment-shader.glsl", GL_FRAGMENT_SHADER, assetManager_p);

		unsigned int shader = glCreateProgram();
		glAttachShader(shader, vertexShader);
		glAttachShader(shader, fragmentShader);
		glLinkProgram(shader);
		
		renderer_p->colorShader = shader;

	}
    /*
	{
		unsigned int vertexShader = getCompiledShader("shaders/renderer2d/color-vertex-shader.glsl", GL_VERTEX_SHADER, assetManager_p);
		unsigned int fragmentShader = getCompiledShader("shaders/renderer2d/texture-fragment-shader.glsl", GL_FRAGMENT_SHADER, assetManager_p);

		unsigned int shader = glCreateProgram();
		glAttachShader(shader, vertexShader);
		glAttachShader(shader, fragmentShader);
		glLinkProgram(shader);
		
		renderer_p->textureShader = shader;

	}
	{
		unsigned int vertexShader = getCompiledShader("shaders/renderer2d/color-vertex-shader.glsl", GL_VERTEX_SHADER, assetManager_p);
		unsigned int fragmentShader = getCompiledShader("shaders/renderer2d/texture-color-fragment-shader.glsl", GL_FRAGMENT_SHADER, assetManager_p);

		unsigned int shader = glCreateProgram();
		glAttachShader(shader, vertexShader);
		glAttachShader(shader, fragmentShader);
		glLinkProgram(shader);
		
		renderer_p->textureColorShader = shader;

	}
     */

}

/*
void Renderer2D_Texture_initFromText(Renderer2D_Texture *texture_p, const char *text, Font font){

	int width, height;
	char *data = getImageDataFromFontAndString_mustFree(font, text, &width, &height);

	Renderer2D_Texture_init(texture_p, text, data, width, height);

	free(data);
	
};
*/

//SETTINGS FUNCTIONS

void Renderer2D_updateDrawSize(Renderer2D_Renderer *renderer_p, int width, int height){

	float newWidth = width;
	float newHeight = height;

	float offsetX = 0;
	float offsetY = 0;

	float aspectRatio = (float)renderer_p->width / (float)renderer_p->height;

	if(newWidth / newHeight > aspectRatio){
		newWidth = height * aspectRatio;
	}else{
		newHeight = width / aspectRatio;
	}

	offsetX = (width - newWidth) / 2;
	offsetY = (height - newHeight) / 2;

	glViewport((int)offsetX, (int)offsetY, (int)newWidth, (int)newHeight);

}

void Renderer2D_setDrawAroundCenter(Renderer2D_Renderer *renderer_p, bool flag){
	renderer_p->drawAroundCenter = flag;
}

//DRAWING FUNCTIONS

void Renderer2D_clear(Renderer2D_Renderer *renderer_p){

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

}

void Renderer2D_setShader(Renderer2D_Renderer *renderer_p, unsigned int shader){
	
	glUseProgram(shader);
	renderer_p->currentShader = shader;
	
}

void Renderer2D_setTexture(Renderer2D_Renderer *renderer_p, Texture texture){

	//glBindTexture(GL_TEXTURE_2D, texture.ID);
	GL3D_uniformTexture(renderer_p->currentShader, "tex", 0, texture.ID);

}

void Renderer2D_setColor(Renderer2D_Renderer *renderer_p, Vec4f color){

	GL3D_uniformVec4f(renderer_p->currentShader, "color", color);

}

void Renderer2D_setRotation(Renderer2D_Renderer *renderer_p, float rotation){

	Mat2f rotationMatrix = getRotationMat2f(rotation);

	GL3D_uniformMat2f(renderer_p->currentShader, "rotationMatrix", rotationMatrix);

}

void Renderer2D_drawRectangle(Renderer2D_Renderer *renderer_p, float x, float y, float width, float height){

	glBindBuffer(GL_ARRAY_BUFFER, renderer_p->rectangleVBO);
	glBindVertexArray(renderer_p->rectangleVAO);

	if(!renderer_p->drawAroundCenter){
		x += width / 2.0;
		y += height / 2.0;
	}

	GL3D_uniformFloat(renderer_p->currentShader, "posX", 2 * ((float)x + renderer_p->offset.x) / (float)renderer_p->height);
	GL3D_uniformFloat(renderer_p->currentShader, "posY", 2 * ((float)y + renderer_p->offset.y) / (float)renderer_p->height);
	GL3D_uniformFloat(renderer_p->currentShader, "width", (float)width / (float)renderer_p->height);
	GL3D_uniformFloat(renderer_p->currentShader, "height", (float)height / (float)renderer_p->height);

	GL3D_uniformFloat(renderer_p->currentShader, "aspectRatio", (float)renderer_p->width / (float)renderer_p->height);

	glDrawArrays(GL_TRIANGLES, 0, 6);

}


void Renderer2D_drawText(Renderer2D_Renderer *renderer_p, const char *text, float x, float y, float fontSize, Font font){

	int textImageWidth, textImageHeight;
	char *textImage = getImageDataFromFontAndString_mustFree(font, text, &textImageWidth, &textImageHeight);

	Texture texture;
	Texture_init(&texture, "text", (unsigned char *)textImage, textImageWidth, textImageHeight);

	free(textImage);

	Renderer2D_setTexture(renderer_p, texture);

	Renderer2D_drawRectangle(renderer_p, x, y, (float)textImageWidth * (fontSize / (float)textImageHeight), fontSize);

	Texture_free(&texture);

}

void Renderer2D_Texture2D_initFromText(Renderer2D_Texture2D *texture_p, const char *text, Font font){

	char *textImage = getImageDataFromFontAndString_mustFree(font, text, &texture_p->width, &texture_p->height);

	Texture_init(&texture_p->texture, text, (unsigned char *)textImage, texture_p->width, texture_p->height);

	free(textImage);
	
}
