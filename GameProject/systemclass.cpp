////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"


SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_Timer = 0;
	m_Sound = 0;

	for (int i = 0; i < 10; i++) {
		isIslandSound[i] = false;
		isHarborSound[i] = false;
	}
}


SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}


bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	InitDirectInput();

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if(!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if(!result)
	{
		return false;
	}

	// Create the fps object.
	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();
	// Create the cpu object.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();
	// Create the timer object.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	m_Sound = new SoundClass;
	if (!m_Sound)
	{
		return false;
	}

	// Initialize the sound object.
	result = m_Sound->Initialize(m_hwnd,
		(CHAR*)"./data/SFX/bgm.wav", (CHAR*)"./data/SFX/flag.wav", (CHAR*)"./data/SFX/container.wav",
		(CHAR*)"./data/SFX/pitch.wav", (CHAR*)"./data/SFX/collision.wav", (CHAR*)"./data/SFX/atmosphere_engine.wav",
		(CHAR*)"./data/SFX/atmosphere_wave.wav", (CHAR*)"./data/SFX/success.wav", (CHAR*)"./data/SFX/fail.wav",
		(CHAR*)"./data/SFX/UIon.wav", (CHAR*)"./data/SFX/UIoff.wav", (CHAR*)"./data/SFX/fire.wav");
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
		return false;
	}
	
	return true;
}


void SystemClass::Shutdown()
{
	// Release the graphics object.
	if(m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the cpu object.
	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	if (m_Sound)
	{
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = 0;
	}


	// Shutdown the window.
	ShutdownWindows();
	
	return;
}


void SystemClass::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	// Loop until there is a quit message from the window or the user.
	done = false;
	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if(!result)
			{
				MessageBox(m_hwnd, L"Frame Processing Failed", L"Error", MB_OK);
				done = true;
			}
		}
		if (m_Input->IsEscapePressed() == true || isGameEnded == true)
		{
			done = true;
		}

	}
	return;
}


bool SystemClass::Frame()
{
	bool result;
	int mouseX, mouseY;

	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	m_fps = m_Fps->GetFps();

	DetectInput();

	m_Sound->volumeSE_Engine((int)m_Graphics->charCamDist * 50);
	m_Sound->volumeSE_Wave((int)m_Graphics->CamHeight * 30);

	result = m_Input->Frame();
	if (!result) {
		return false;
	}
	m_Input->GetMouseLocation(mouseX, mouseY);
	// Check if the user pressed escape and wants to exit the application.
	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame(m_fps, m_Cpu->GetCpuPercentage(), m_Timer->GetTime(), mouseX, mouseY, speed * 200.0f, pitch);
	if(!result)
	{
		return false;
	}

	if (m_Graphics->isGameScene) {
		if (gameSceneSoundPlayed == false) {
			m_Sound->playBGM();
			m_Sound->playSE_Engine();
			m_Sound->playSE_Wave();
			gameSceneSoundPlayed = true;
		}
	}
	else
	{
		gameSceneSoundPlayed = false;
		m_Sound->stopFile(1);
		m_Sound->stopFile(6);
		m_Sound->stopFile(7);
	}

	if ((m_fps != 0)) {
		fps_Calculate = round((144.0f / m_fps) * 10) / 10;
		m_Graphics->m_fps_Calculate = fps_Calculate;
	}

	else {
		m_Graphics->m_fps_Calculate = 1.0f;
	}
	for (int i = 0; i < m_Graphics->num_island; i++) {
		if ((m_Graphics->isCollide_island[i])) {
			if (isIslandSound[i] == false) {
				m_Sound->playCollision();
				isIslandSound[i] = true;
			}
		}
		else {
			isIslandSound[i] = false;
		}
	}

	for (int i = 0; i < m_Graphics->num_harbor; i++) {
		if ((m_Graphics->isCollide_harbor[i])) {
			if (isHarborSound[i] == false) {
				m_Sound->playCollision();
				isHarborSound[i] = true;
			}
		}
		else {
			isHarborSound[i] = false;
		}
	}
	if ((m_Graphics->isBoundary)) {
		if (isBoundarySound == false) {
			m_Sound->playCollision();
			isBoundarySound = true;
		}
	}
	else {
		isBoundarySound = false;
	}

	if (m_Graphics->clear) {
		if (isClearSound == false) {
			m_Sound->playSuccess();
			isClearSound = true;
		}
	}
	if (m_Graphics->fail) {
		if (isFailSound == false) {
			m_Sound->playFail();
			isFailSound = true;
		}
	}
	return true;
}


LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}


void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Naval Warfare Simulation";

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hinstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth  = 1600;
		screenHeight = 900;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_POPUP,
						    posX, posY, screenWidth+30, screenHeight+30, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}


void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}


bool SystemClass::InitDirectInput()
{
	DirectInput8Create(m_hinstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&DirectInput,
		NULL);

	DirectInput->CreateDevice(GUID_SysKeyboard,
		&DIKeyboard,
		NULL);

	DirectInput->CreateDevice(GUID_SysMouse,
		&DIMouse,
		NULL);

	DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	DIKeyboard->SetCooperativeLevel(m_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	DIMouse->SetDataFormat(&c_dfDIMouse);
	DIMouse->SetCooperativeLevel(m_hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

	return true;
}

void SystemClass::DetectInput()
{
	DIMOUSESTATE mouseCurrState;

	BYTE keyboardState[256];

	DIKeyboard->Acquire();
	DIMouse->Acquire();

	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	if (keyboardState[DIK_1] & 0X80) {
		m_Graphics->isGameScene = false;
		m_Graphics->isMaintitle = true;
		m_Graphics->isResultScene = false;
		turnRate = 0;
		pitch = 0;
		speed = 0;
		isFired = false;
	}

	if (keyboardState[DIK_2] & 0X80) {
		m_Graphics->isGameScene = true;
		m_Graphics->isMaintitle = false;
		m_Graphics->isResultScene = false;
		isFired = false;
	}

	if (keyboardState[DIK_3] & 0X80) {
		m_Graphics->isGameScene = false;
		m_Graphics->isMaintitle = false;
		m_Graphics->isResultScene = true;
		turnRate = 0;
		pitch = 0;
		speed = 0;
		isFired = false;
	}

	if (m_Graphics->isMenuPressed == true || m_Graphics->isRetryMenuPressed == true) {
		if (keyboardState[DIK_N] & 0X80) {
			m_Sound->playUIOff();
			m_Graphics->isMenuPressed = false;
			m_Graphics->isRetryMenuPressed = false;
		}
	}

	if (m_Graphics->isMaintitle) {
		if (keyboardState[DIK_RETURN] & 0X80) {
			m_Graphics->isGameScene = true;
			m_Graphics->isMaintitle = false;
			m_Graphics->isResultScene = false;
			isFired = false;
			m_Sound->playUIOff();
		}

		if ((keyboardState[DIK_ESCAPE] & 0X80) && isESCPressed == false) {
			if (m_Graphics->isMenuPressed == false) {
				m_Sound->playUIOn();
			}
			m_Graphics->isMenuPressed = true;
			isESCPressed = true;
		}

		if (!(keyboardState[DIK_ESCAPE] & 0X80)) {
			isESCPressed = false;
		}

		if (m_Graphics->isMenuPressed == true) {
			if (keyboardState[DIK_Y] & 0X80) {
				m_Graphics->isMenuPressed = false;

				m_Sound->playUIOff();
				isGameEnded = true;
			}
		}
	}

	if (m_Graphics->isResultScene) {
		if ((keyboardState[DIK_ESCAPE] & 0X80) && isESCPressed == false) {
			m_Sound->playUIOff();
			m_Graphics->isGameScene = false;
			m_Graphics->isMaintitle = true;
			m_Graphics->isResultScene = false;
			turnRate = 0;
			pitch = 0;
			speed = 0;
			isFired = false;
			isESCPressed = true;
		}

		if (!(keyboardState[DIK_ESCAPE] & 0X80)) {
			isESCPressed = false;
		}
	}

	if (m_Graphics->isGameScene) {

		if ((keyboardState[DIK_ESCAPE] & 0X80) && isESCPressed == false) {
			if (m_Graphics->isMenuPressed == false) {
				m_Sound->playUIOn();
			}
			m_Graphics->isMenuPressed = true;
			isESCPressed = true;
		}

		if (!(keyboardState[DIK_ESCAPE] & 0X80)) {
			isESCPressed = false;
		}

		if (m_Graphics->isMenuPressed == true) {
			if (keyboardState[DIK_Y] & 0X80) {
				m_Graphics->isMenuPressed = false;

				m_Sound->playUIOff();
				m_Graphics->isGameScene = false;
				m_Graphics->isMaintitle = false;
				m_Graphics->isResultScene = true;
				turnRate = 0;
				pitch = 0;
				speed = 0;
				isFired = false;
			}
		}

		if ((keyboardState[DIK_R] & 0X80) && isRPressed == false) {
			if (m_Graphics->isRetryMenuPressed == false) {
				m_Sound->playUIOn();
			}
			m_Graphics->isRetryMenuPressed = true;
			isRPressed = true;
		}

		if (!(keyboardState[DIK_R] & 0X80)) {
			isRPressed = false;
		}

		if (m_Graphics->isRetryMenuPressed == true) {
			if (keyboardState[DIK_Y] & 0X80) {
				m_Graphics->isRetryMenuPressed = false;

				m_Sound->playUIOff();
				m_Graphics->isRetryPressed = true;
				turnRate = 0;
				pitch = 0;
				speed = 0;
				currTimeforFire = 6.0f;
				lastTimeforFire = 0.0f;
				isFired = false;
			}
		}

		if ((keyboardState[DIK_C] & 0X80) && isFreeLookCam == false && isCPressed == false) {
			isCPressed = true;
			isFreeLookCam = true;
			m_Graphics->isFreeLookMode = true;
		}

		if ((keyboardState[DIK_C] & 0X80) && isFreeLookCam == true && isCPressed == false) {
			isCPressed = true;
			isFreeLookCam = false;
			m_Graphics->isFreeLookMode = false;
		}

		if (!(keyboardState[DIK_C] & 0X80)) {
			isCPressed = false;
		}

		currTimeforFire = m_Graphics->m_time;
		m_Graphics->reloadTime = (int)(currTimeforFire - lastTimeforFire);

		if ((currTimeforFire - lastTimeforFire) >= 5.0f) {
			isFired = false;
		}

		if ((mouseCurrState.rgbButtons[0] & 0x80) && mouseClicked == false)
		{
			if (!isFired) {
				if (m_Graphics->isAimed) {
					m_Sound->playFire((int)m_Graphics->charCamDist * 10);

					isFired = true;
					lastTimeforFire = m_Graphics->m_time;
				}
				m_Graphics->shooting = true;
			}
			mouseClicked = true;
		}
		else if ((mouseCurrState.rgbButtons[0] & 0x80) && mouseClicked == true)
		{
			m_Graphics->shooting = false;
		}
		if (!(mouseCurrState.rgbButtons[0] & 0x80)) {
			m_Graphics->shooting = false;
			mouseClicked = false;
		}

		if (!isFreeLookCam) {
			if (!(keyboardState[DIK_D] & 0x80) && !(keyboardState[DIK_A] & 0x80)) {
				if (m_Graphics->playerRoll > 0)
					m_Graphics->playerRoll -= 0.0005f;
				if (m_Graphics->playerRoll < 0)
					m_Graphics->playerRoll += 0.0005f;
				if (m_Graphics->playerRoll == 0)
					m_Graphics->playerRoll = 0.0f;

				if (turnRate > 0)
					turnRate = turnRate - turnRateAccel * m_Graphics->m_fps_Calculate;
				if (turnRate < 0)
					turnRate = turnRate + turnRateAccel * m_Graphics->m_fps_Calculate;
			}

			if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY)
				|| (mouseCurrState.lZ != mouseLastState.lZ))
			{
				m_Graphics->camYaw += mouseLastState.lX * 0.001f;
				if ((m_Graphics->camPitch <= XMConvertToRadians(10)) && (m_Graphics->camPitch >= -XMConvertToRadians(60))) {
					m_Graphics->camPitch -= mouseCurrState.lY * 0.001f;
				}
				if (m_Graphics->camPitch > XMConvertToRadians(10)) {
					m_Graphics->camPitch = XMConvertToRadians(10);
				}
				if (m_Graphics->camPitch < -XMConvertToRadians(60)) {
					m_Graphics->camPitch = -XMConvertToRadians(60);
				}

				if ((m_Graphics->charCamDist <= 80.0f) && (m_Graphics->charCamDist >= 10.0f)) {
					m_Graphics->charCamDist -= mouseCurrState.lZ * 0.03f;
				}

				if ((m_Graphics->charCamDist > 80.0f)) {
					m_Graphics->charCamDist += mouseCurrState.lZ * 0.03f;
				}

				if ((m_Graphics->charCamDist < 10.0f)) {
					m_Graphics->charCamDist += mouseCurrState.lZ * 0.03f;
				}

				if ((m_Graphics->charCamHeight <= 16.0f) && (m_Graphics->charCamHeight >= 2.0f)) {
					m_Graphics->charCamHeight -= mouseCurrState.lZ * 0.006f;
				}

				if ((m_Graphics->charCamHeight > 16.0f)) {
					m_Graphics->charCamHeight += mouseCurrState.lZ * 0.006f;
				}

				if ((m_Graphics->charCamHeight < 2.0f)) {
					m_Graphics->charCamHeight += mouseCurrState.lZ * 0.006f;
				}

				mouseLastState = mouseCurrState;
			}

			if (speed > 0.01 && pitch > 0) {
				if (keyboardState[DIK_A] & 0x80)
				{
					if (!(turnRate < -0.001)) {
						turnRate = turnRate - turnRateAccel * (speed / (speedGap * 1.5f)) * m_Graphics->m_fps_Calculate;
					}
					if (m_Graphics->playerRoll < 0.1f && pitch != 0) {
						m_Graphics->playerRoll += 0.001f;
					}
				}
				if (keyboardState[DIK_D] & 0x80)
				{
					if (!(turnRate > 0.001)) {
						turnRate = turnRate + turnRateAccel * (speed / (speedGap * 1.5f)) * m_Graphics->m_fps_Calculate;
					}
					if (m_Graphics->playerRoll > -0.1f && pitch != 0) {
						m_Graphics->playerRoll -= 0.001f;
					}
				}
				m_Graphics->playerYaw += turnRate * m_Graphics->m_fps_Calculate;
			}

			else if (speed < -0.01 && pitch < 0) {
				if (keyboardState[DIK_D] & 0x80)
				{
					if (!(turnRate < -0.001)) {
						turnRate = turnRate + turnRateAccel * (speed / speedGap) * m_Graphics->m_fps_Calculate;
					}
					if (m_Graphics->playerRoll > -0.1f && pitch != 0) {
						m_Graphics->playerRoll -= 0.001f;
					}
				}
				if (keyboardState[DIK_A] & 0x80)
				{
					if (!(turnRate > 0.001)) {
						turnRate = turnRate - turnRateAccel * (speed / speedGap) * m_Graphics->m_fps_Calculate;
					}
					if (m_Graphics->playerRoll < 0.1f && pitch != 0) {
						m_Graphics->playerRoll += 0.001f;
					}

				}
				m_Graphics->playerYaw += turnRate * m_Graphics->m_fps_Calculate;
			}
			else {
				if (m_Graphics->playerRoll > 0.001)
					m_Graphics->playerRoll -= 0.0005f;
				else if (m_Graphics->playerRoll < 0.001)
					m_Graphics->playerRoll += 0.0005f;
				else
					m_Graphics->playerRoll = 0.0f;

				if (turnRate > 0)
					turnRate = turnRate - turnRateAccel * m_Graphics->m_fps_Calculate;
				if (turnRate < 0)
					turnRate = turnRate + turnRateAccel * m_Graphics->m_fps_Calculate;
				m_Graphics->playerYaw += turnRate * m_Graphics->m_fps_Calculate;
			}

			m_Graphics->turnSpeed = turnRate;

			if ((keyboardState[DIK_W] & 0x80) && onceKeyDowned == false)
			{
				if (pitch < 3) {
					pitch++;
					m_Sound->playPitch();
				}
				onceKeyDowned = true;
			}
			if ((keyboardState[DIK_S] & 0x80) && onceKeyDowned == false)
			{
				if (pitch > -1) {
					pitch--;
					m_Sound->playPitch();
				}
				onceKeyDowned = true;
			}

			if (!(keyboardState[DIK_W] & 0x80) && !(keyboardState[DIK_S] & 0x80) && onceKeyDowned == true)
			{
				onceKeyDowned = false;
			}

			if (speed < (speedGap * pitch)) {
				speed = speed + speedAccel * m_Graphics->m_fps_Calculate;
			}
			else if (speed == (speedGap * pitch)) {
				speed = speed;
			}
			else {
				speed = speed - speedAccel * m_Graphics->m_fps_Calculate;
			}

			////////////////충돌 판정 부분//////////////////

			if (speed > 0.001f) {
				m_Graphics->currentSpeedState = FRONT;
				if ((keyboardState[DIK_W] & 0x80)) {
					m_Graphics->collisionBack = false;
					m_Graphics->collisionFront = false;
				}
			}
			else if (speed < 0.001f && speed > -0.001f) {
				m_Graphics->currentSpeedState = STOP;
			}
			else {
				m_Graphics->currentSpeedState = BACK;
				if ((keyboardState[DIK_S] & 0x80)) {
					m_Graphics->collisionBack = false;
					m_Graphics->collisionFront = false;
				}
			}
			m_Graphics->moveBackForward -= speed * m_Graphics->m_fps_Calculate;

			if (!(keyboardState[DIK_SPACE] & 0X80)) {
				isSpacePressed = false;
				m_Graphics->spacePressTime = 0.0f;
			}

			if (m_Graphics->collisionWithContainer) {
				if (isContainerSound == false) {
					m_Sound->playFlag();
					isContainerSound = true;
				}

				if ((keyboardState[DIK_SPACE] & 0X80) && (isSpacePressed == false)) {
					lastTime = m_Graphics->m_time;
					isSpacePressed = true;
				}
				if ((keyboardState[DIK_SPACE] & 0X80) && (isSpacePressed == true)) {
					currTime = m_Graphics->m_time;
					m_Graphics->spacePressTime = currTime - lastTime;
					if ((currTime - lastTime) > 3.0f) {
						m_Sound->playContainer();
					}
				}
			}
			else {
				isContainerSound = false;
			}
		}

		else {
			
			///자유시점 카메라 토글 on 시에 카메라 제어
			if (!(keyboardState[DIK_A] & 0x80) || !(keyboardState[DIK_D] & 0x80)) {
				m_Graphics->CAMmoveLeftRight = 0;
			}
			if (!(keyboardState[DIK_W] & 0x80) || !(keyboardState[DIK_S] & 0x80)) {
				m_Graphics->CAMmoveBackForward = 0;
			}

			if (keyboardState[DIK_A] & 0x80)
			{
				m_Graphics->CAMmoveLeftRight -= speed4FreeLook * m_Graphics->m_fps_Calculate;
			}
			if (keyboardState[DIK_D] & 0x80)
			{
				m_Graphics->CAMmoveLeftRight += speed4FreeLook * m_Graphics->m_fps_Calculate;
			}
			if (keyboardState[DIK_W] & 0x80)
			{
				m_Graphics->CAMmoveBackForward += speed4FreeLook * m_Graphics->m_fps_Calculate;
			}
			if (keyboardState[DIK_S] & 0x80)
			{
				m_Graphics->CAMmoveBackForward -= speed4FreeLook * m_Graphics->m_fps_Calculate;
			}
			if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
			{
				m_Graphics->camYaw += mouseLastState.lX * 0.001f;

				m_Graphics->camPitch += mouseCurrState.lY * 0.001f;

				mouseLastState = mouseCurrState;
			}

		}
		if (m_Graphics->collisionWithFlag) {
			m_Sound->playFlag();
		}
	}
	else {
		currTimeforFire = 6.0f;
		lastTimeforFire = 0.0f;
	}

	return;
}