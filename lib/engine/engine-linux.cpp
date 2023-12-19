#ifdef __linux__

#include "engine/engine.h"
#include "engine/strings.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "time.h"
#include "unistd.h"

#include <vector>
#include <chrono>

#include "glad/gl.h"

#include "X11/X.h"
#include "X11/Xlib.h"
#include "X11/extensions/Xfixes.h"
#include "X11/XKBlib.h"
#include <X11/Xutil.h>
#include "GL/glx.h"

Display *dpy;
int screenNumber;
Window root;
GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo *vi;
Colormap cmap;
XSetWindowAttributes swa;
Window win;
GLXContext glc;
XWindowAttributes gwa;
XEvent xev;

typedef GLXContext (*glXCreateContextAttribsARBProc)
    (Display*, GLXFBConfig, GLXContext, Bool, const int*);

unsigned int OS_KEY_IDENTIFIERS[] = {

	XK_0,
	XK_1,
	XK_2,
	XK_3,
	XK_4,
	XK_5,
	XK_6,
	XK_7,
	XK_8,
	XK_9,

	XK_A,
	XK_B,
	XK_C,
	XK_D,
	XK_E,
	XK_F,
	XK_G,
	XK_H,
	XK_I,
	XK_J,
	XK_K,
	XK_L,
	XK_M,
	XK_N,
	XK_O,
	XK_P,
	XK_Q,
	XK_R,
	XK_S,
	XK_T,
	XK_U,
	XK_V,
	XK_W,
	XK_X,
	XK_Y,
	XK_Z,

	XK_Up,
	XK_Down,
	XK_Left,
	XK_Right,

	XK_space,
	XK_Escape,

	XK_Shift_L,
	XK_Control_L,

};

int main(){

	//setup window
	dpy = XOpenDisplay(NULL);

	if(dpy == NULL){
		printf("Cannot open X display!\n");
		return 0;
	}

	root = DefaultRootWindow(dpy);
	screenNumber = DefaultScreen(dpy);

	vi = glXChooseVisual(dpy, 0, att);

	if(vi == NULL){
		printf("Could not Choose X Visual\n");
		return 0;
	}

	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

	swa.colormap = cmap;
	//swa.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | StructureNotifyMask | ButtonReleaseMask | PointerMotionMask;
	swa.event_mask = StructureNotifyMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask;

	win = XCreateWindow(dpy, root, 0, 0, Engine_clientWidth, Engine_clientHeight, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

	XMapWindow(dpy, win);

	XStoreName(dpy, win, "Untitled Engine Program");

	static int visual_attribs[] = {
		GLX_X_RENDERABLE    , True,
		GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
		GLX_RENDER_TYPE     , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_RED_SIZE        , 8,
		GLX_GREEN_SIZE      , 8,
		GLX_BLUE_SIZE       , 8,
		GLX_ALPHA_SIZE      , 8,
		GLX_DEPTH_SIZE      , 24,
		GLX_STENCIL_SIZE    , 8,
		GLX_DOUBLEBUFFER    , True,
		GLX_SAMPLE_BUFFERS  , 1,
		GLX_SAMPLES         , 4,
		None
    };

    int num_fbc = 0;
    GLXFBConfig *fbc = glXChooseFBConfig(dpy, DefaultScreen(dpy), visual_attribs, &num_fbc);

    if (!fbc) {
        printf("glXChooseFBConfig() failed\n");
        exit(1);
    }

	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB =
        (glXCreateContextAttribsARBProc)
        glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");

	static int context_attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        None
    };

printf("got here\n");

	glc = glXCreateContextAttribsARB(dpy, fbc[0], NULL, GL_TRUE, context_attribs);
	glXMakeCurrent(dpy, win, glc);

printf("not here\n");

	gladLoadGL((GLADloadfunc)glXGetProcAddress);

	//int screen = DefaultScreen(dpy);

	//glXSwapIntervalEXT(1);

	Atom wmDelete = XInternAtom(dpy, "WM_DELETE_WINDOW", true);
	XSetWMProtocols(dpy, win, &wmDelete, 1);

	int autoRepeatIsAvailable;
	XkbSetDetectableAutoRepeat(dpy, true, &autoRepeatIsAvailable);

	//init keycodes
	for(int i = 0; i < ENGINE_KEYS_LENGTH; i++){
		Engine_keys[i].OSIdentifier = OS_KEY_IDENTIFIERS[i];
	}

	//common inits
	//initPixelDrawing();
	internal_initKeys();
	internal_initPointer();

	Engine_start();

	//game loop
	size_t startTicks = 0;
	size_t endTicks = 0;

	size_t deltaTime = 0;
	size_t accumilatedTime = 0;
	size_t frameTime = 1000000 / 60;

	int fps = 60;

	size_t ticksPerFrame = CLOCKS_PER_SEC / fps;

	//bool quit = false;

	bool cursorHidden = false;
	bool lastFocused = false;
	bool focused = false;

	while(!internal_programShouldQuit){

		//startTicks = clock();
		auto frameStartTime = std::chrono::high_resolution_clock::now();

		//check window focus
		{
			lastFocused = focused;
			focused = false;

			Window focusedWindow;
			int returnInt;
			XGetInputFocus(dpy, &focusedWindow, &returnInt);

			if(focusedWindow == win){
				focused = true;
			}
		}

		//printf("%i\n", focused);

		if(focused){

			//handle window and button press events
			while(XPending(dpy) > 0){

				XNextEvent(dpy, &xev);

				if(xev.type == ClientMessage
				|| xev.type == DestroyNotify){
					internal_programShouldQuit = true;
				}

				if(xev.type == ConfigureNotify){

					XConfigureEvent xce = xev.xconfigure;

					if(xce.width != Engine_clientWidth
					|| xce.height != Engine_clientHeight){
						Engine_clientWidth = xce.width;
						Engine_clientHeight = xce.height;
					}

				}

				if(xev.type == ButtonPress){
					XButtonEvent *buttonEvent_p = (XButtonEvent *)&xev;

					if(buttonEvent_p->button == 1){
						Engine_pointer.down = true;
						Engine_pointer.downed = true;
						Engine_pointer.lastDownedPos = Engine_pointer.pos;
					}

				}
				if(xev.type == ButtonRelease){

					XButtonEvent *buttonEvent_p = (XButtonEvent *)&xev;

					if(buttonEvent_p->button == 1){
						Engine_pointer.down = false;
						Engine_pointer.upped = true;
						Engine_pointer.lastUppedPos = Engine_pointer.pos;
					}

					if(buttonEvent_p->button == 4){
						Engine_pointer.scroll++;
					}
					if(buttonEvent_p->button == 5){
						Engine_pointer.scroll--;
					}

				}

				if(xev.type == KeyPress){

					char buffer[SMALL_STRING_SIZE];
					String_set(buffer, "", SMALL_STRING_SIZE);

					XLookupString((XKeyPressedEvent *)&xev, buffer, STRING_SIZE, NULL, NULL);

					if(!(strcmp(buffer, "") == 0)){
						Engine_textInput.push_back(*buffer);
					}

				}

			}

			//get keyboard state
			{
				char keys[32];
				XQueryKeymap(dpy, keys);

				for(int i = 0; i < ENGINE_KEYS_LENGTH; i++){

					int keyCode = XKeysymToKeycode(dpy, OS_KEY_IDENTIFIERS[i]);
					int byteIndex = keyCode / 8;
					int bitIndex = keyCode % 8;

					if((keys[byteIndex] >> bitIndex) & 0x01){
						if(!Engine_keys[i].down){
							Engine_keys[i].downed = true;
						}
						Engine_keys[i].down = true;
					}else{
						if(Engine_keys[i].down){
							Engine_keys[i].upped = true;
						}
						Engine_keys[i].down = false;
					}
					
				}
			}

			//get pointer state
			{
				Window returnWindow;
				int returnInt;
				int XPointerX, XPointerY;
				unsigned int buttonMask;
				if(XQueryPointer(dpy, win, &returnWindow, &returnWindow, &returnInt, &returnInt, &XPointerX, &XPointerY, &buttonMask)){

					Engine_pointer.pos.x = XPointerX;
					Engine_pointer.pos.y = XPointerY;

					Engine_pointer.movement.x = Engine_pointer.pos.x - Engine_clientWidth / 2;
					Engine_pointer.movement.y = Engine_pointer.pos.y - Engine_clientHeight / 2;

				}
			}

			//do fps magic
			if(Engine_fpsModeOn){

				/*
				if(!lastFocused){
					XFixesHideCursor(dpy, root);
					XFlush(dpy);
				}
				*/

				int screenWidth = DisplayWidth(dpy, DefaultScreen(dpy));
				int screenHeight = DisplayHeight(dpy, DefaultScreen(dpy));

				XWarpPointer(dpy, None, root, 0, 0, 0, 0, screenWidth / 2, screenHeight / 2);

			}

		}

		//handle cursor visibility
		if(focused
		&& Engine_fpsModeOn){
			if(!cursorHidden){
				XFixesHideCursor(dpy, root);
				XFlush(dpy);
				cursorHidden = true;
			}
		}else{
			if(cursorHidden){
				XFixesShowCursor(dpy, root);
				XFlush(dpy);
				cursorHidden = false;
			}
		}

		/*
		if(!focused
		&& lastFocused
		&& Engine_fpsModeOn){
			XFixesShowCursor(dpy, root);
			XFlush(dpy);
		}
		*/

		//update

		//while(accumilatedTime > frameTime){
		
		auto startTime = std::chrono::high_resolution_clock::now();

		Engine_update();

		auto stopTime = std::chrono::high_resolution_clock::now();

		long int updateTime = (long int)(std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime).count());

			//accumilatedTime -= frameTime;

		internal_resetKeys();
		internal_resetPointer();
			
		//}

		//draw

		startTime = std::chrono::high_resolution_clock::now();

		Engine_draw();

		glXSwapBuffers(dpy, win);

		glFinish();

		stopTime = std::chrono::high_resolution_clock::now();

		long int drawTime = (long int)(std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime).count());

		Engine_elapsedFrames++;

		//endTicks = clock();
		auto frameStopTime = std::chrono::high_resolution_clock::now();

		//deltaTime = (endTicks - startTicks) / (CLOCKS_PER_SEC / 1000000);
		long int totalFrameTime = (long int)(std::chrono::duration_cast<std::chrono::microseconds>(frameStopTime - frameStartTime).count());

		Engine_frameUpdateTime = (float)updateTime / 1000.0;
		Engine_frameDrawTime = (float)drawTime / 1000.0;
		Engine_frameTime = (float)totalFrameTime / 1000.0;

		//printf("\nframeTime: %i\n", deltaTime);

		//printf("%f\n", (float)deltaTime / 1000);

		int lag = frameTime - totalFrameTime;

		//printf("lag: %i\n", lag);

		if(lag < 0){
			lag = 0;
		}

		usleep(lag);

		/*
		if(ENGINE_PRINT_FRAME_TIME){

			printf("\n\nENGINE TIMINGS\n");

			printf("\nEngine_update() time: %f ms\n", (float)updateTime / 1000.0);

			printf("Engine_draw() time: %f ms\n\n", (float)drawTime / 1000.0);

			printf("total frame time: %f ms\n\n", (float)totalFrameTime / 1000.0);

		}
		*/

		//accumilatedTime += deltaTime;

		//printf("%i\n", deltaTime);

		//int lag = ticksPerFrame - (endTicks - startTicks);

		//if(lag < 0){
			//lag = 0;
		//}

		//float lagMilliseconds = (float)lag / (float)(CLOCKS_PER_SEC / 1000);

		//usleep(lagMilliseconds * 1000);

	
	}

	Engine_finnish();

	return 0;

}

void Engine_setWindowSize(int width, int height){

	Engine_clientWidth = width;
	Engine_clientHeight = height;

	XResizeWindow(dpy, win, width, height);

}

void Engine_centerWindow(){

	XMoveWindow(dpy, win, DisplayWidth(dpy, screenNumber) / 2 - Engine_clientWidth / 2, DisplayHeight(dpy, screenNumber) / 2 - Engine_clientHeight / 2);

}

void Engine_toggleFullscreen(){

	if(!Engine_isFullscreen){

		Engine_setWindowSize(DisplayWidth(dpy, screenNumber), DisplayHeight(dpy, screenNumber));
		Engine_centerWindow();

		Engine_isFullscreen = true;
	}else{

		Engine_setWindowSize(480 * 2, 270 * 2);
		Engine_centerWindow();
	
		Engine_isFullscreen = false;
	}

}

#endif
