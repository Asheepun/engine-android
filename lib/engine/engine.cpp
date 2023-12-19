//common includes
#include "engine/engine.h"
#include "engine/strings.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

#include <vector>
#include <chrono>

//#include "glad/glad.h"

#include "glad/gl.h"

//linux includes
#ifdef __linux__

#include "time.h"
#include "unistd.h"

#include "X11/X.h"
#include "X11/Xlib.h"
#include "X11/extensions/Xfixes.h"
#include "X11/XKBlib.h"
#include <X11/Xutil.h>
#include "GL/glx.h"

#endif

//windows includes
#ifdef _WIN32

#include "glad/wgl.h"

#include "time.h"
#include <limits.h>
#include <winnt.h>

#include "windows.h"
#include "winuser.h"

//#include "glad/glad_wgl.h"

#endif

bool ENGINE_PRINT_FRAME_TIME = true;
//bool ENGINE_PRINT_FRAME_TIME = false;

#ifdef __linux__

#endif

#ifdef _WIN32
HWND hwnd;
#endif

//GLOBAL VARIABLE DEFINITIONS

//Engine_Pixel colorBuffers[ENGINE_COLORS_LENGTH][COLOR_BUFFER_SIZE];
//int screenWidth = 800;
//int screenHeight = 450;
//Engine_Pixel *screenPixels = NULL;
//int windowWidth = 800;
//int windowHeight = 450;
int Engine_clientWidth = 800;
int Engine_clientHeight = 450;
bool Engine_isFullscreen = false;

float Engine_frameUpdateTime = 0.0;
float Engine_frameDrawTime = 0.0;
float Engine_frameTime = 0.0;

int Engine_elapsedFrames = 0;

bool Engine_fpsModeOn = false;

Engine_Key Engine_keys[ENGINE_KEYS_LENGTH];

Engine_Pointer Engine_pointer;

std::vector<char> Engine_textInput;
//Array Engine_textInput;

#ifdef __linux__
#endif

#ifdef _WIN32
static unsigned int internal_OS_KEY_IDENTIFIERS[] = {

	0x30,
	0x31,
	0x32,
	0x33,
	0x34,
	0x35,
	0x36,
	0x37,
	0x38,
	0x39,

	0x41,
	0x42,
	0x43,
	0x44,
	0x45,
	0x46,
	0x47,
	0x48,
	0x49,
	0x4A,
	0x4B,
	0x4C,
	0x4D,
	0x4E,
	0x4F,
	0x50,
	0x51,
	0x52,
	0x53,
	0x54,
	0x55,
	0x56,
	0x57,
	0x58,
	0x59,
	0x5A,

	VK_UP,
	VK_DOWN,
	VK_LEFT,
	VK_RIGHT,

	VK_SPACE,
	VK_ESCAPE,

	VK_SHIFT,
	VK_CONTROL,

};
#endif

bool internal_programShouldQuit = false;

//COMMON INITS
void internal_initKeys(){

	for(int i = 0; i < ENGINE_KEYS_LENGTH; i++){

		Engine_keys[i].down = false;
		Engine_keys[i].downed = false;
		Engine_keys[i].upped = false;
	
	}
	
}

void internal_resetKeys(){
	for(int i = 0; i < ENGINE_KEYS_LENGTH; i++){
		Engine_keys[i].downed = false;
		Engine_keys[i].upped = false;
	}

	Engine_textInput.clear();
	//Array_clear(&Engine_textInput);
}

void internal_initPointer(){
	Engine_pointer.pos = getVec2f(0, 0);
	Engine_pointer.down = false;
	Engine_pointer.downed = false;
	Engine_pointer.upped = false;
	Engine_pointer.scroll = 0;
}

void internal_resetPointer(){
	Engine_pointer.downed = false;
	Engine_pointer.upped = false;
	Engine_pointer.scroll = 0;
}

//ENGINE ENTRY POINT

#ifdef __linux__
#endif

#ifdef _WIN32
//bool QUIT_PROGRAM = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	
	//setup window
	const char CLASS_NAME[] = "Untitled Engine Program";
	
	WNDCLASS wc = {};
	
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	
	RegisterClass(&wc);
	
	hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		"Untitled Engine Program",
		WS_OVERLAPPEDWINDOW,

		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		
		NULL,
		NULL,
		hInstance,
		NULL
	);
	
	if(hwnd == NULL){
		printf("Could not create Window");
		return 0;
	}
	
	HDC hdc = GetDC(hwnd);
	
	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	
	int pf = ChoosePixelFormat(hdc, &pfd);
	
	if(pf == 0){
		printf("Could not choose pixel format\n");
		return 0;
	}
	
	SetPixelFormat(hdc, pf, &pfd);
	
	DescribePixelFormat(hdc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	
	HGLRC hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	gladLoaderLoadWGL(hdc);
	gladLoaderLoadGL();
	//gladLoadWGL(hdc);

	wglSwapIntervalEXT(1);

	printf("%s\n", glGetString(GL_VERSION));
	//printf("%s\n", glGetString(GL_EXTENSIONS));
	//printf("%s\n", wglGetExtensionsStringARB());

	//wglSwapIntervalEXT(0);

	//common inits
	//initPixelDrawing();
	initKeys();
	initPointer();
	
	Engine_start();
	
	ShowWindow(hwnd, nCmdShow);

	LARGE_INTEGER liFrequency = {0};
	LARGE_INTEGER liStart = {0};
	LARGE_INTEGER liStop = {0};

	float deltaTime;

	float accumilatedTime = 0;

	bool cursorHidden = false;
	bool lastFocused = false;
	bool focused = false;

	//game loop
	while(!programShouldQuit){

		QueryPerformanceFrequency(&liFrequency);

		QueryPerformanceCounter(&liStart);

		//check window focus
		{
			lastFocused = focused;
			focused = false;

			HWND focusedWindow = GetFocus();

			if(focusedWindow == hwnd){
				focused = true;
			}
		
		}
	
		//handle events
		MSG msg = {};
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
		
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		
		}

		//do fps magic
		if(Engine_fpsModeOn
		&& focused){

			RECT windowRect;
			RECT clientRect;
			GetWindowRect(hwnd, &windowRect);
			GetClientRect(hwnd, &clientRect);

			int smallMargin = (windowRect.right - windowRect.left - clientRect.right) / 2;
			int largeMargin = windowRect.bottom - windowRect.top - clientRect.bottom - smallMargin;

			SetCursorPos(windowRect.left + smallMargin + Engine_clientWidth / 2, windowRect.top + largeMargin + Engine_clientHeight / 2);

		}

		//handle cursor visibility
		if(focused
		&& Engine_fpsModeOn){
			while(ShowCursor(false) >= 0){}
		}else{
			while(ShowCursor(true) <= 0){}
		}

		//update
			
		while(accumilatedTime > 1000 / 60){

			Engine_update();

			accumilatedTime -= 1000 / 60;

			resetKeys();
			resetPointer();

			//printf("hello from here %f\n", accumilatedTime);
		
		}
		
		//draw
		
		Engine_draw();
		
		SwapBuffers(hdc);
		
		glFinish();
		
		//glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE, screenPixels);

		Engine_elapsedFrames++;

		QueryPerformanceCounter(&liStop);

		deltaTime = (float)((liStop.QuadPart - liStart.QuadPart) * 1000000 / liFrequency.QuadPart) / 1000;

		accumilatedTime += deltaTime;
		
		//printf("%f\n", deltaTime);

	}

	Engine_finnish();
	
	return 0;
	
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if(uMsg == WM_CLOSE
	|| uMsg == WM_DESTROY
	|| uMsg == WM_QUIT){
		PostQuitMessage(0);
		programShouldQuit = true;
		return 0;
	}

	if(uMsg == WM_KEYDOWN){
		
		for(int i = 0; i < ENGINE_KEYS_LENGTH; i++){
			if(wParam == Engine_keys[i].OSIdentifier){
				if(!Engine_keys[i].down){
					Engine_keys[i].downed = true;
				}
				Engine_keys[i].down = true;
			}
		}

	}

	if(uMsg == WM_KEYUP){
		
		for(int i = 0; i < ENGINE_KEYS_LENGTH; i++){
			if(wParam == Engine_keys[i].OSIdentifier){
				if(Engine_keys[i].down){
					Engine_keys[i].upped = true;
				}
				Engine_keys[i].down = false;
			}
		}

		
	}

	if(uMsg == WM_LBUTTONDOWN){
		Engine_pointer.down = true;
		Engine_pointer.downed = true;
		Engine_pointer.lastDownedPos = Engine_pointer.pos;
	}

	if(uMsg == WM_LBUTTONUP){
		Engine_pointer.down = false;
		Engine_pointer.upped = true;
		Engine_pointer.lastUppedPos = Engine_pointer.pos;
	}

	if(uMsg == WM_MOUSEMOVE){

		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		Engine_pointer.pos.x = x;
		Engine_pointer.pos.y = y;

		Engine_pointer.movement.x = x - Engine_clientWidth / 2;
		Engine_pointer.movement.y = y - Engine_clientHeight / 2;

	}

	if(uMsg == WM_MOUSEWHEEL){

		short delta = GET_WHEEL_DELTA_WPARAM(wParam);

		Engine_pointer.scroll = delta / 120;

	}

	if(uMsg == WM_SIZE){

		Engine_clientWidth = LOWORD(lParam);
		Engine_clientHeight = HIWORD(lParam);

		//printf("%i\n", GetLastError());

		//if(result){
			//printf("%f\n", lpRect->right);
		//
		//}

		//clientWidth = (int)lpRect->right;
		//clientHeight = lpRect->bottom - lpRect->top;

	}

    return DefWindowProc(hwnd, uMsg, wParam, lParam);

}
#endif

//ENGINE FUNCTIONS

void Engine_quit(){
	internal_programShouldQuit = true;
}

//WINDOW FUNCTIONS
void Engine_setWindowTitle(char *title){
	
#ifdef _WIN32
	SetWindowTextA(hwnd, (LPCSTR)title);
#endif
	
}

/*
void Engine_setWindowSize(int width, int height){

	//windowWidth = width;
	//windowHeight = height;
	Engine_clientWidth = width;
	Engine_clientHeight = height;

#ifdef __linux__
	XResizeWindow(dpy, win, width, height);
#endif

#ifdef _WIN32

	
	RECT rect;
	rect.left = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	rect.top = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	SetWindowPos(hwnd, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
#endif

	//screenPixels = malloc(sizeof(Engine_Pixel) * width * height);

}
*/


/*
void Engine_toggleFullscreen(){

#ifdef _WIN32
	if(!Engine_isFullscreen){
		SetWindowLongPtrA(hwnd, -16, WS_VISIBLE);

		SetWindowPos(hwnd, NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);

		Engine_isFullscreen = true;
	}else{
		SetWindowLongPtrA(hwnd, -16, WS_OVERLAPPEDWINDOW);
		Engine_setWindowSize(480 * 2, 270 * 2);

		Engine_isFullscreen = false;
	}
#endif

#ifdef __linux__
	if(!Engine_isFullscreen){

		Engine_setWindowSize(DisplayWidth(dpy, screenNumber), DisplayHeight(dpy, screenNumber));
		Engine_centerWindow();

		Engine_isFullscreen = true;
	}else{

		Engine_setWindowSize(480 * 2, 270 * 2);
		Engine_centerWindow();
	
		Engine_isFullscreen = false;
	}
#endif

}
*/
