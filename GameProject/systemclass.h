////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN


//////////////
// INCLUDES //
//////////////
#include <windows.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "graphicsclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "timerclass.h"
#include "soundclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();
	void DetectInput();
	bool InitDirectInput();

	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;

	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 DirectInput;

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	int m_screenWidth;
	int m_screenHeight;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	TimerClass* m_Timer;

	SoundClass* m_Sound;

	bool isGameEnded = false;

	bool gameSceneSoundPlayed = false;

	bool onceKeyDowned = false;
	bool isSpacePressed = false;
	bool isRPressed = false;
	bool isESCPressed = false;

	bool mouseClicked = false;
	bool isFired = false;

	float currTimeforFire = 6.0f;
	float lastTimeforFire = 0.0f;

	bool isIslandSound[10];
	bool isHarborSound[10];
	bool isBoundarySound = false;
	bool isContainerSound = false;
	bool isFailSound = false;
	bool isClearSound = false;
	bool isESConSound = false;
	bool isESCoffSound = false;

	float currTime = 0.0f;
	float lastTime = 0.0f;

	float turnRate = 0.0f; //max 0.001 min -0.001
	float turnRateAccel = 0.0001f * 0.01745f;

	int pitch = 0; // -1 ~ 3
	float speed = 0.0f;
	float speedGap = 0.05f;
	float speedAccel = 0.0002f;
	int m_fps = 1;

	int fps_Cal_Divide = 0;

	float fps_Calculate = 0.0f;
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;


#endif