////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;

	m_SkyMap = 0;

	m_Sea = 0;
	m_Seafloor = 0;
	m_Player = 0;
	m_Turret = 0;
	m_Island = 0;
	m_Harbor = 0;
	m_lHouse = 0;
	m_Rocks = 0;
	m_Flag = 0;
	m_Container = 0;
	m_Convoy[0] = 0;
	m_Convoy[1] = 0;
	m_Convoy[2] = 0;
	m_CoastGuardBase = 0;
	m_CoastGuardCannon[0] = 0;
	m_CoastGuardCannon[1] = 0;
	m_CoastGuardCannon[2] = 0;
	m_Fire[0] = 0;
	m_Fire[1] = 0;
	m_Fire[2] = 0;

	m_convoyFire[0] = 0;
	m_convoyFire[1] = 0;
	m_convoyFire[2] = 0;

	m_baseSmoke[0] = 0;
	m_baseSmoke[1] = 0;
	m_baseSmoke[2] = 0;

	m_convoySmoke[0] = 0;
	m_convoySmoke[1] = 0;
	m_convoySmoke[2] = 0;

	m_Smoke_1 = 0;
	m_Smoke_2 = 0;
	m_Smoke_3 = 0;

	m_Crosshair = 0;
	m_Crosshair_Aimed = 0;
	m_Crosshair_Arm = 0;
	m_Crosshair_Aimed_Arm = 0;
	m_UIBG = 0;
	m_UIBG2 = 0;
	m_shipUI = 0;
	m_SNUI = 0;
	m_turnUI = 0;
	m_turnArrowUI = 0;
	m_PressSpace = 0;
	m_HoldBar = 0;
	m_HoldBarFrame = 0;
	m_CollisionWarning = 0;
	m_Clear = 0;
	m_Fail = 0;
	m_Menu = 0;
	m_UIBGforTitle = 0;
	m_ResultScene = 0;
	m_MainTitle = 0;
	m_inGameMenu = 0;
	m_RetrytMenu = 0;

	m_Bullet[0] = 0;
	m_Bullet[1] = 0;
	m_Bullet[2] = 0;

	m_Text = 0;

	m_TextureShader = 0;
	m_TextureBlendShader = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_RefractionTexture = 0;
	m_ReflectionTexture = 0;
	m_RefractionShader = 0;
	m_WaterShader = 0;
	m_ClipPlaneShaderClass = 0;

	for (int i = 0; i < 3; i++) {
		Vector_Turret[i] = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		Vector_Turret_Forward[i] = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		Vector_Turret_Right[i] = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
		Vector_Turret_Up[i] = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		convoyHit[i] = false;
		coastGuardCannonHit[i] = false;
	}
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	XMMATRIX baseViewMatrix;

	screenwidth = screenWidth;
	screenheight = screenHeight;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	m_Camera->setSize(0.5f, 0.5f, 0.5f);

	// 파티클 셰이더 개체를 만듭니다.
	m_SmokeShaderClass = new ParticleShaderClass;
	if (!m_SmokeShaderClass)
	{
		return false;
	}

	// 파티클 셰이더 개체를 초기화합니다.
	if (!m_SmokeShaderClass->Initialize(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the particle shader object.", L"Error", MB_OK);
		return false;
	}

	for (int i = 0; i < 3; i++) {
		m_baseSmoke[i] = new ParticleSystemClass;
		if (!m_baseSmoke[i])
		{
			return false;
		}

		// 파티클 시스템 객체를 초기화합니다.
		if (!m_baseSmoke[i]->Initialize(m_D3D->GetDevice(), L"./data/particles/smoke.dds", TYPE_1))
		{
			return false;
		}

		m_convoySmoke[i] = new ParticleSystemClass;
		if (!m_convoySmoke[i])
		{
			return false;
		}

		// 파티클 시스템 객체를 초기화합니다.
		if (!m_convoySmoke[i]->Initialize(m_D3D->GetDevice(), L"./data/particles/smoke.dds", TYPE_1))
		{
			return false;
		}
	}

	// 파티클 시스템 객체를 만듭니다.
	m_Smoke_1 = new ParticleSystemClass;
	if (!m_Smoke_1)
	{
		return false;
	}

	// 파티클 시스템 객체를 초기화합니다.
	if (!m_Smoke_1->Initialize(m_D3D->GetDevice(), L"./data/particles/smoke.dds", TYPE_1))
	{
		return false;
	}

	m_Smoke_2 = new ParticleSystemClass;
	if (!m_Smoke_2)
	{
		return false;
	}

	// 파티클 시스템 객체를 초기화합니다.
	if (!m_Smoke_2->Initialize(m_D3D->GetDevice(), L"./data/particles/signalSmoke.dds", TYPE_2))
	{
		return false;
	}

	m_Smoke_3 = new ParticleSystemClass;
	if (!m_Smoke_2)
	{
		return false;
	}

	// 파티클 시스템 객체를 초기화합니다.
	if (!m_Smoke_3->Initialize(m_D3D->GetDevice(), L"./data/particles/signalSmoke.dds", TYPE_3))
	{
		return false;
	}

	for (int i = 0; i < 3; i++) {
		// 모델 객체 생성
		m_Fire[i] = new FireModelClass;
		if (!m_Fire[i])
		{
			return false;
		}

		// 모델 객체 초기화
		if (!m_Fire[i]->Initialize(m_D3D->GetDevice(), "./data/models/square.txt", L"./data/textures/fire01.dds",
			L"./data/textures/noise01.dds", L"./data/textures/alpha01.dds"))
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_convoyFire[i] = new FireModelClass;
		if (!m_convoyFire[i])
		{
			return false;
		}

		// 모델 객체 초기화
		if (!m_convoyFire[i]->Initialize(m_D3D->GetDevice(), "./data/models/square.txt", L"./data/textures/fire01.dds",
			L"./data/textures/noise01.dds", L"./data/textures/alpha01.dds"))
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
	}

	// 파이어 쉐이더 객체를 생성한다.
	m_FireShader = new FireShaderClass;
	if (!m_FireShader)
	{
		return false;
	}

	// 화재 쉐이더 객체를 초기화합니다.
	if (!m_FireShader->Initialize(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the fire shader object.", L"Error", MB_OK);
		return false;
	}
		
	// Create the model object.
	m_Player = new ModelClass;
	if (!m_Player)
	{
		return false;
	}

	result = m_Player->Initialize(m_D3D->GetDevice(), (char*)"./data/models/petropavlovsk.obj", (WCHAR*)L"./data/textures/petropavlovsk.dds");

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Flag = new ModelClass;
	if (!m_Flag)
	{
		return false;
	}

	result = m_Flag->Initialize(m_D3D->GetDevice(), (char*)"./data/models/flag.obj", (WCHAR*)L"./data/textures/flag.dds");

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Container = new ModelClass;
	if (!m_Container)
	{
		return false;
	}

	result = m_Container->Initialize(m_D3D->GetDevice(), (char*)"./data/models/pileBox.obj", (WCHAR*)L"./data/textures/pileBox.dds");

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	for (int i = 0; i < 3; i++) {
		m_Convoy[i] = new ModelClass;
		if (!m_Convoy[i])
		{
			return false;
		}

		result = m_Convoy[i]->Initialize(m_D3D->GetDevice(), (char*)"./data/models/convoy.obj", (WCHAR*)L"./data/textures/convoy.dds");

		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_CoastGuardCannon[i] = new ModelClass;
		if (!m_CoastGuardCannon[i])
		{
			return false;
		}

		result = m_CoastGuardCannon[i]->Initialize(m_D3D->GetDevice(), (char*)"./data/models/coastGuardCannon.obj", (WCHAR*)L"./data/textures/coastGuard.dds");

		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
	}

	m_CoastGuardBase = new ModelClass;
	if (!m_CoastGuardBase)
	{
		return false;
	}

	result = m_CoastGuardBase->Initialize(m_D3D->GetDevice(), (char*)"./data/models/coastGuardBase.obj", (WCHAR*)L"./data/textures/coastGuard.dds");

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_lHouse = new ModelClass;
	if (!m_lHouse)
	{
		return false;
	}

	result = m_lHouse->Initialize(m_D3D->GetDevice(), (char*)"./data/models/lighthouse.obj", (WCHAR*)L"./data/textures/lighthouse.dds");

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Rocks = new ModelClass;
	if (!m_Rocks)
	{
		return false;
	}

	result = m_Rocks->Initialize(m_D3D->GetDevice(), (char*)"./data/models/weedRock.obj", (WCHAR*)L"./data/textures/weedRock.dds");

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Harbor = new ModelClass;
	if (!m_Harbor)
	{
		return false;
	}

	result = m_Harbor->Initialize(m_D3D->GetDevice(), (char*)"./data/models/harbor.obj", (WCHAR*)L"./data/textures/harbor.dds");

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Turret = new ModelClass;
	if (!m_Turret)
	{
		return false;
	}

	result = m_Turret->Initialize(m_D3D->GetDevice(), (char*)"./data/models/turret.obj", (WCHAR*)L"./data/textures/turret.dds");

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	for(int i = 0; i < 3; i++) {
		m_Bullet[i] = new ProjectileClass;
		if (!m_Bullet)
		{
			return false;
		}

		result = m_Bullet[i]->Initialize(m_D3D->GetDevice(), (char*)"./data/models/projectile.obj", (WCHAR*)L"./data/textures/projectile.dds");
	}

	m_Sea = new ModelClass;
	if (!m_Sea)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Sea->Initialize(m_D3D->GetDevice(), (char*)"./data/models/sea.obj", (WCHAR*)L"./data/textures/sea.dds");

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Seafloor = new ModelClass;
	if (!m_Sea)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Seafloor->Initialize(m_D3D->GetDevice(), (char*)"./data/models/seafloor.obj", (WCHAR*)L"./data/textures/seafloor.dds");

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_SkyMap = new SkymapClass;
	if (!m_SkyMap)
	{
		return false;
	}

	// Initialize the skymap object.
	result = m_SkyMap->Initialize(m_D3D->GetDevice(), (WCHAR*)L"./data/textures/skymap.dds", 10, 10);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Skymap object.", L"Error", MB_OK);
		return false;
	}

	m_Island = new ModelClass;
	if (!m_Island)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Island->Initialize(m_D3D->GetDevice(), (char*)"./data/models/island.obj", (WCHAR*)L"./data/textures/island.dds");

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	m_Light->SetAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.8f, -1.5f, 3.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(128.0f);

	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	m_TextureBlendShader = new TextureBlendShaderClass;
	if (!m_TextureBlendShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureBlendShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Crosshair = new BitmapClass;
	if (!m_Crosshair)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Crosshair->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/crosshair.dds", screenwidth * 0.006, screenwidth * 0.006);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_Crosshair_Aimed = new BitmapClass;
	if (!m_Crosshair_Aimed)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Crosshair_Aimed->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/crosshair_aimed.dds", screenwidth * 0.006, screenwidth * 0.006);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Crosshair_Arm = new BitmapClass;
	if (!m_Crosshair)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Crosshair_Arm->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/crosshair.dds", screenwidth * 0.1, screenwidth * 0.002);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_Crosshair_Aimed_Arm = new BitmapClass;
	if (!m_Crosshair_Aimed)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Crosshair_Aimed_Arm->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/crosshair_aimed.dds", screenwidth * 0.1, screenwidth * 0.002);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_UIBG = new BitmapClass;
	if (!m_UIBG)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_UIBG->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/UIBG.dds", 205, 288);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_UIBG2 = new BitmapClass;
	if (!m_UIBG2)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_UIBG2->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/UIBG.dds", 285, 200);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_SNUI = new BitmapClass;
	if (!m_SNUI)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_SNUI->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/SNUI.dds", 175, 175);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_shipUI = new BitmapClass;
	if (!m_shipUI)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_shipUI->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/shipUI.dds", 100, 100);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_turnUI = new BitmapClass;
	if (!m_SNUI)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_turnUI->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/turnUI.dds", 500, 100);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_turnArrowUI = new BitmapClass;
	if (!m_shipUI)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_turnArrowUI->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/turnArrowUI.dds", 24, 24);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_PressSpace = new BitmapClass;
	if (!m_PressSpace)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_PressSpace->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/PressSpace.dds", 300, 200);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_HoldBar = new BitmapClass;
	if (!m_HoldBar)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_HoldBar->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/holdBar.dds", 200, 400);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_HoldBarFrame = new BitmapClass;
	if (!m_HoldBarFrame)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_HoldBarFrame->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/holdBarFrame.dds", 200, 400);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_CollisionWarning = new BitmapClass;
	if (!m_CollisionWarning)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_CollisionWarning->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/Collision.dds", 200, 200);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_Clear = new BitmapClass;
	if (!m_Clear)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Clear->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/Clear.dds", 400, 200);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_Fail = new BitmapClass;
	if (!m_Fail)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Fail->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/Fail.dds", 400, 200);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_Menu = new BitmapClass;
	if (!m_Menu)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Menu->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/Menu.dds", 400, 300);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_UIBGforTitle = new BitmapClass;
	if (!m_UIBGforTitle)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_UIBGforTitle->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/UIBG_for_Titles.dds", screenWidth , screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_ResultScene = new BitmapClass;
	if (!m_ResultScene)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_ResultScene->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/ResultScene.dds", screenHeight * 1.25f, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_MainTitle = new BitmapClass;
	if (!m_MainTitle)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_MainTitle->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/MainTitle.dds", screenHeight * 1.25f, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_RetrytMenu = new BitmapClass;
	if (!m_RetrytMenu)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_RetrytMenu->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/MenuRetry.dds", 400, 300);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_inGameMenu = new BitmapClass;
	if (!m_inGameMenu)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_inGameMenu->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/textures/MenuInGame.dds", 400, 300);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	

	// Create the refraction render to texture object.
	m_RefractionTexture = new RenderTextureClass;
	if (!m_RefractionTexture)
	{
		return false;
	}

	// Initialize the refraction render to texture object.
	result = m_RefractionTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the reflection render to texture object.
	m_ReflectionTexture = new RenderTextureClass;
	if (!m_ReflectionTexture)
	{
		return false;
	}

	// Initialize the reflection render to texture object.
	result = m_ReflectionTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the reflection render to texture object.", L"Error", MB_OK);
		return false;
	}

	m_ClipPlaneShaderClass = new ClipPlaneShaderClass;
	if (!m_ClipPlaneShaderClass)
	{
		return false;
	}

	// Initialize the refraction shader object.
	result = m_ClipPlaneShaderClass->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the refraction shader object.
	m_RefractionShader = new RefractionShaderClass;
	if (!m_RefractionShader)
	{
		return false;
	}

	// Initialize the refraction shader object.
	result = m_RefractionShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the water shader object.
	m_WaterShader = new WaterShaderClass;
	if (!m_WaterShader)
	{
		return false;
	}

	// Initialize the water shader object.
	result = m_WaterShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the water shader object.", L"Error", MB_OK);
		return false;
	}

	// Set the height of the water.
	m_waterHeight = 0.0f;

	// Initialize the position of the water.
	m_waterTranslation = 0.0f;

	m_Camera->Render(playerYaw, playerPitch, moveLeftRight, moveBackForward, CamDefaultForward, CamDefaultRight, playerPosition, charCamDist, charCamHeight);
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}
	m_D3D->GetWorldMatrix(worldMatrix_skymap);

	m_D3D->GetWorldMatrix(worldMatrix_Player);
	m_D3D->GetWorldMatrix(worldMatrix_sea);
	m_D3D->GetWorldMatrix(worldMatrix_seafloor);

	for (int i = 0; i < 9; i++) {
		m_D3D->GetWorldMatrix(worldMatrix_island[i]); // 지형지물 및 오브젝트
	}

	for (int i = 0; i < 5; i++) {
		m_D3D->GetWorldMatrix(worldMatrix_Rocks[i]); // 지형지물 및 오브젝트
	}

	for (int i = 0; i < 5; i++) {
		m_D3D->GetWorldMatrix(worldMatrix_harbor[i]); // 지형지물 및 오브젝트
	}

	for (int i = 0; i < 3; i++) {
		m_D3D->GetWorldMatrix(worldMatrix_lHouse[i]); // 지형지물 및 오브젝트
	}

	for (int i = 0; i < 3; i++) {
		m_D3D->GetWorldMatrix(worldMatrix_Bullet[i]);
	}

	m_D3D->GetWorldMatrix(worldMatrix_Smoke);
	m_D3D->GetWorldMatrix(worldMatrix_Smoke_2);
	m_D3D->GetWorldMatrix(worldMatrix_Smoke_3);

	m_D3D->GetWorldMatrix(worldMatrix_Text);
	m_D3D->GetWorldMatrix(worldMatrix_shipUI);
	m_D3D->GetWorldMatrix(worldMatrix_TurnBar);
	m_D3D->GetWorldMatrix(worldMatrix_CH);
	m_D3D->GetWorldMatrix(worldMatrix_CHA);
	for (int i = 0; i < 2; i++) {
		m_D3D->GetWorldMatrix(worldMatrix_CH_arm[i]);
		m_D3D->GetWorldMatrix(worldMatrix_CHA_arm[i]);
	}

	ArmDis = screenwidth * 0.2;
	aimedArmDis = (screenwidth * 0.2) / 2;

	return true;
}


void GraphicsClass::Shutdown()
{
	if (m_FireShader)
	{
		m_FireShader->Shutdown();
		delete m_FireShader;
		m_FireShader = 0;
	}

	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_TextureBlendShader)
	{
		m_TextureBlendShader->Shutdown();
		delete m_TextureBlendShader;
		m_TextureBlendShader = 0;
	}

	// Release the model object.
	if (m_SkyMap)
	{
		m_SkyMap->Shutdown();
		delete m_SkyMap;
		m_SkyMap = 0;
	}

	if (m_Rocks)
	{
		m_Rocks->Shutdown();
		delete m_Rocks;
		m_Rocks = 0;
	}

	if (m_Harbor)
	{
		m_Harbor->Shutdown();
		delete m_Harbor;
		m_Harbor = 0;
	}

	if (m_Player)
	{
		m_Player->Shutdown();
		delete m_Player;
		m_Player = 0;
	}

	if (m_Flag)
	{
		m_Flag->Shutdown();
		delete m_Flag;
		m_Flag = 0;
	}

	if (m_lHouse)
	{
		m_lHouse->Shutdown();
		delete m_lHouse;
		m_lHouse = 0;
	}

	if (m_Turret)
	{
		m_Turret->Shutdown();
		delete m_Turret;
		m_Turret = 0;
	}

	for (int i = 0; i < 3; i++) {
		if (m_Bullet[i])
		{
			m_Bullet[i]->Shutdown();
			delete m_Bullet[i];
			m_Bullet[i] = 0;
		}

		if (m_baseSmoke[i])
		{
			m_baseSmoke[i]->Shutdown();
			delete m_baseSmoke[i];
			m_baseSmoke[i] = 0;
		}

		if (m_convoySmoke[i])
		{
			m_convoySmoke[i]->Shutdown();
			delete m_convoySmoke[i];
			m_convoySmoke[i] = 0;
		}
	}

	for (int i = 0; i < 3; i++) {
		if (m_Fire[i])
		{
			m_Fire[i]->Shutdown();
			delete m_Fire[i];
			m_Fire[i] = 0;
		}

		if (m_convoyFire[i])
		{
			m_convoyFire[i]->Shutdown();
			delete m_convoyFire[i];
			m_convoyFire[i] = 0;
		}
	}

	if (m_Sea)
	{
		m_Sea->Shutdown();
		delete m_Sea;
		m_Sea = 0;
	}

	if (m_Container)
	{
		m_Container->Shutdown();
		delete m_Container;
		m_Container = 0;
	}

	for (int i = 0; i < 3; i++) {
		if (m_Convoy[i])
		{
			m_Convoy[i]->Shutdown();
			delete m_Convoy[i];
			m_Convoy[i] = 0;
		}

		if (m_CoastGuardCannon[i])
		{
			m_CoastGuardCannon[i]->Shutdown();
			delete m_CoastGuardCannon[i];
			m_CoastGuardCannon[i] = 0;
		}
	}

	if (m_CoastGuardBase)
	{
		m_CoastGuardBase->Shutdown();
		delete m_CoastGuardBase;
		m_CoastGuardBase = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the water shader object.
	if (m_WaterShader)
	{
		m_WaterShader->Shutdown();
		delete m_WaterShader;
		m_WaterShader = 0;
	}

	// Release the refraction shader object.
	if (m_RefractionShader)
	{
		m_RefractionShader->Shutdown();
		delete m_RefractionShader;
		m_RefractionShader = 0;
	}

	if (m_ClipPlaneShaderClass)
	{
		m_ClipPlaneShaderClass->Shutdown();
		delete m_ClipPlaneShaderClass;
		m_ClipPlaneShaderClass = 0;
	}

	// Release the reflection render to texture object.
	if (m_ReflectionTexture)
	{
		m_ReflectionTexture->Shutdown();
		delete m_ReflectionTexture;
		m_ReflectionTexture = 0;
	}

	// Release the refraction render to texture object.
	if (m_RefractionTexture)
	{
		m_RefractionTexture->Shutdown();
		delete m_RefractionTexture;
		m_RefractionTexture = 0;
	}

	// Release the bitmap object.
	if (m_Crosshair)
	{
		m_Crosshair->Shutdown();
		delete m_Crosshair;
		m_Crosshair = 0;
	}

	if (m_Crosshair_Aimed)
	{
		m_Crosshair_Aimed->Shutdown();
		delete m_Crosshair_Aimed;
		m_Crosshair_Aimed = 0;
	}

	if (m_Crosshair_Arm)
	{
		m_Crosshair_Arm->Shutdown();
		delete m_Crosshair_Arm;
		m_Crosshair_Arm = 0;
	}

	if (m_Crosshair_Aimed_Arm)
	{
		m_Crosshair_Aimed_Arm->Shutdown();
		delete m_Crosshair_Aimed_Arm;
		m_Crosshair_Aimed_Arm = 0;
	}

	if (m_UIBG)
	{
		m_UIBG->Shutdown();
		delete m_UIBG;
		m_UIBG = 0;
	}

	if (m_UIBG2)
	{
		m_UIBG2->Shutdown();
		delete m_UIBG2;
		m_UIBG2 = 0;
	}

	if (m_shipUI)
	{
		m_shipUI->Shutdown();
		delete m_shipUI;
		m_shipUI = 0;
	}

	if (m_SNUI)
	{
		m_SNUI->Shutdown();
		delete m_SNUI;
		m_SNUI = 0;
	}

	if (m_turnUI)
	{
		m_turnUI->Shutdown();
		delete m_turnUI;
		m_turnUI = 0;
	}

	if (m_turnArrowUI)
	{
		m_turnArrowUI->Shutdown();
		delete m_turnArrowUI;
		m_turnArrowUI = 0;
	}

	if (m_SmokeShaderClass)
	{
		m_SmokeShaderClass->Shutdown();
		delete m_SmokeShaderClass;
		m_SmokeShaderClass = 0;
	}

	if (m_Smoke_1)
	{
		m_Smoke_1->Shutdown();
		delete m_Smoke_1;
		m_Smoke_1 = 0;
	}

	if (m_Smoke_2)
	{
		m_Smoke_2->Shutdown();
		delete m_Smoke_2;
		m_Smoke_2 = 0;
	}

	if (m_Smoke_3)
	{
		m_Smoke_3->Shutdown();
		delete m_Smoke_3;
		m_Smoke_3 = 0;
	}

	if (m_PressSpace)
	{
		m_PressSpace->Shutdown();
		delete m_PressSpace;
		m_PressSpace = 0;
	}

	if (m_HoldBar)
	{
		m_HoldBar->Shutdown();
		delete m_HoldBar;
		m_HoldBar = 0;
	}

	if (m_HoldBarFrame)
	{
		m_HoldBarFrame->Shutdown();
		delete m_HoldBarFrame;
		m_HoldBarFrame = 0;
	}

	if (m_CollisionWarning)
	{
		m_CollisionWarning->Shutdown();
		delete m_CollisionWarning;
		m_CollisionWarning = 0;
	}

	if (m_Clear)
	{
		m_Clear->Shutdown();
		delete m_Clear;
		m_Clear = 0;
	}

	if (m_Fail)
	{
		m_Fail->Shutdown();
		delete m_Fail;
		m_Fail = 0;
	}

	if (m_Menu)
	{
		m_Menu->Shutdown();
		delete m_Menu;
		m_Menu = 0;
	}

	if (m_UIBGforTitle)
	{
		m_UIBGforTitle->Shutdown();
		delete m_UIBGforTitle;
		m_UIBGforTitle = 0;
	}

	if (m_ResultScene)
	{
		m_ResultScene->Shutdown();
		delete m_ResultScene;
		m_ResultScene = 0;
	}

	if (m_MainTitle)
	{
		m_MainTitle->Shutdown();
		delete m_MainTitle;
		m_MainTitle = 0;
	}

	if (m_RetrytMenu)
	{
		m_RetrytMenu->Shutdown();
		delete m_RetrytMenu;
		m_RetrytMenu = 0;
	}

	if (m_inGameMenu)
	{
		m_inGameMenu->Shutdown();
		delete m_inGameMenu;
		m_inGameMenu = 0;
	}
	
	return;
}

bool GraphicsClass::Frame(int fps, int cpu, float frameTime, int mouseX, int mouseY, float m_speed, int m_pitch)
{
	bool result;

	this->frameTime = frameTime;
	m_mouseX = mouseX;
	m_mouseY = mouseY;

	speed = ceil(m_speed);
	pitch = m_pitch;

	if (fps <= 0) m_time = 0;
	else m_time += 1.0f / fps;

	m_waterTranslation += 0.0001f * m_fps_Calculate;
	if (m_waterTranslation > 1.0f) m_waterTranslation -= 1.0f;

	if (m_time < 210) {
		if (wayPointNum == 1) {
			clear = true;
		}
		fail = false;
	}
	else {
		if (!clear) {
			fail = true;
		}
	}

	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	result = m_Text->SetTime(m_time, m_D3D->GetDeviceContext());
	result = m_Text->SetObject(num_total, m_D3D->GetDeviceContext());
	result = m_Text->SetScreenWidth(m_D3D->GetDeviceContext());
	result = m_Text->SetScreenHeight(m_D3D->GetDeviceContext());
	result = m_Text->SetPolygon(m_indexCnt, m_D3D->GetDeviceContext());
	result = m_Text->SetMousePosition(m_mouseX, m_mouseY, m_D3D->GetDeviceContext());
	result = m_Text->SetAimState(isAimed, m_D3D->GetDeviceContext());
	result = m_Text->SetPitchState(m_pitch, m_D3D->GetDeviceContext());
	result = m_Text->SetSpeedState(speed, m_D3D->GetDeviceContext());
	result = m_Text->SetQuestState(clear, fail, m_D3D->GetDeviceContext());
	result = m_Text->SetFlagsState(flagNum, fail, m_D3D->GetDeviceContext());
	result = m_Text->SetContainersState(containerNum, fail, m_D3D->GetDeviceContext());
	result = m_Text->SetResultState(clear, fail, m_D3D->GetDeviceContext());
	result = m_Text->SetReloadState(reloadTime, m_D3D->GetDeviceContext());

	m_Smoke_1->Frame(frameTime, m_D3D->GetDeviceContext(), speed / 200.0f, TYPE_1);
	m_Smoke_2->Frame(frameTime, m_D3D->GetDeviceContext(), speed / 200.0f, TYPE_2);
	m_Smoke_3->Frame(frameTime, m_D3D->GetDeviceContext(), speed / 200.0f, TYPE_3);

	for (int i = 0; i < 3; i++) {
		if (coastGuardCannonHit[i]) {
			m_baseSmoke[i]->Frame(frameTime, m_D3D->GetDeviceContext(), speed / 200.0f, TYPE_4);
		}
		if (convoyHit[i]) {
			m_convoySmoke[i]->Frame(frameTime, m_D3D->GetDeviceContext(), speed / 200.0f, TYPE_4);
		}
	}

	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}

void GraphicsClass::SetPosition() {

	/// <플레이어 이동>
	/// ////////

	camRotateInfoYaw = playerYaw;
	camRotateInfoPitch = playerPitch;

	if ((collisionFront)) {
		if (moveBackForward < -0.01f) {
			rotateMatrix_Player = MoveChar(playerYaw, playerPitch, playerRoll, moveLeftRight, 0,
				DefaultForward, DefaultRight);
		}
		else {
			rotateMatrix_Player = MoveChar(playerYaw, playerPitch, playerRoll, moveLeftRight, moveBackForward,
				DefaultForward, DefaultRight);
		}
	}
	else if ((collisionBack)) {
		if (moveBackForward > 0.01f) {
			rotateMatrix_Player = MoveChar(playerYaw, playerPitch, playerRoll, moveLeftRight, 0,
				DefaultForward, DefaultRight);
		}
		else {
			rotateMatrix_Player = MoveChar(playerYaw, playerPitch, playerRoll, moveLeftRight, moveBackForward,
				DefaultForward, DefaultRight);
		}
	}
	else {
		rotateMatrix_Player = MoveChar(playerYaw, playerPitch, playerRoll, moveLeftRight, moveBackForward,
			DefaultForward, DefaultRight);
	}

	printf("%d, %d \n",(int)m_playerPosition.x, (int)m_playerPosition.z);

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	XMStoreFloat3(&m_playerPosition, playerPosition);

	if (accelate_Assend > accelate_Assend_Limit || m_playerPosition.y > accelate_Tilt_Limit) { isAssend = false; }
	if (accelate_Assend < -accelate_Assend_Limit || m_playerPosition.y < -accelate_Tilt_Limit) { isAssend = true; }
	if (isAssend == false) { accelate_Assend = accelate_Assend - 0.00001f * m_fps_Calculate; }
	if (isAssend == true) { accelate_Assend = accelate_Assend + 0.00001f * m_fps_Calculate; }

	if (accelate_Tilt > accelate_Tilt_Limit || playerPitch > accelate_Tilt_Limit) { isTilt = false; }
	if (accelate_Tilt < -accelate_Tilt_Limit || playerPitch < -accelate_Tilt_Limit) { isTilt = true; }
	if (isTilt == false) { accelate_Tilt -= 0.00003f * 0.0174532925f * m_fps_Calculate; }
	if (isTilt == true) { accelate_Tilt += 0.00003f * 0.0174532925f * m_fps_Calculate; }

	m_playerPosition.y = m_playerPosition.y + accelate_Assend;
	playerPitch = playerPitch + accelate_Tilt;
	playerPosition = XMLoadFloat3(&m_playerPosition);

	m_Player->SetCollider(scaleMatrix_Player_for_Collider, transMatrix_Player);

	transMatrix_Turret[0] = XMMatrixTranslation(0.0f, transTurretY[0], transTurretZ[0]);
	transMatrix_Turret[1] = XMMatrixTranslation(0.0f, transTurretY[1], transTurretZ[1]);
	transMatrix_Turret[2] = XMMatrixTranslation(0.0f, transTurretY[2], transTurretZ[2]);

	for (int i = 0; i < 3; i++) {
		 Vector_Turret_Forward[i] = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		 Vector_Turret_Right[i] = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

		 Vector_Turret_Right[i] = XMVector3TransformCoord(DefaultRight, rotateMatrix_Player);
		 Vector_Turret_Forward[i] = XMVector3TransformCoord(DefaultForward, rotateMatrix_Player);
		 Vector_Turret_Up[i] = XMVector3Cross(Vector_Turret_Forward[i], Vector_Turret_Right[i]);

		 //Vector_Turret[i] += moveLeftRight * Vector_Turret_Right[i];
		 Vector_Turret[i] = (transTurretZ[i] * Vector_Turret_Forward[i]) + ((transTurretY[i] + 1.5f) * Vector_Turret_Up[i]);
	}

	num_turret = 3;

	rotateRad = LookAt(-CamLookDir, DefaultForward, DefaultRight, playerYaw);
	if (turretRad != rotateRad) {
		if ((int)XMConvertToDegrees(rotateRad) - 2 < (int)XMConvertToDegrees(turretRad) &&
			(int)XMConvertToDegrees(turretRad) < (int)XMConvertToDegrees(rotateRad) + 2) {
			isAimed = true;
		}
		else { 
			isAimed = false;
		}

		if (turretRad < rotateRad) {
			turretRad = turretRad + turretTurnRate * m_fps_Calculate;
		}
		else {
			turretRad = turretRad - turretTurnRate * m_fps_Calculate;
		}
	}

	rotateMatrix_Turret[0] = XMMatrixRotationY(turretRad);
	rotateMatrix_Turret[1] = XMMatrixRotationY(turretRad);
	rotateMatrix_Turret[2] = XMMatrixRotationY(turretRad);
	
	for (int i = 0; i < 3; i++) {
		worldMatrix_Turret[i] = scaleMatrix_Turret * rotateMatrix_Turret[i] * transMatrix_Turret[i] * rotateMatrix_Player * transMatrix_Player;
	}

	if (isAimed && shooting) {
		for (int i = 0; i < 3; i++) {
			m_Bullet[i]->shooted = true;
			m_Bullet[i]->inWater = false;
		}
	}
	else {
		for (int i = 0; i < 3; i++) {
			m_Bullet[i]->shooted = false;
		}
	}

	for (int i = 0; i < 3; i++) {
		worldMatrix_Bullet[i] = m_Bullet[i]->ProjectileShoot(playerPosition + Vector_Turret[i]
			, camYaw, camPitch);
		m_Bullet[i]->SetCollider();
	}


	//스카이박스 행렬

	XMFLOAT3 Camera_Pos = m_Camera->GetPosition();
	XMFLOAT3 Camera_Rotate = m_Camera->GetRotation();

	scaleMatrix_skymap = XMMatrixScaling(500.0f, 500.0f, 500.0f);
	transMatrix_skymap = XMMatrixTranslation(Camera_Pos.x, Camera_Pos.y - 10.0f, Camera_Pos.z);
	rotateMatrix_skymap = XMMatrixRotationY(256 * 0.0174532925f);

	worldMatrix_skymap = scaleMatrix_skymap * rotateMatrix_skymap * transMatrix_skymap;

	scaleMatrix_sea = XMMatrixScaling(2000.0f, 0.001f, 2000.0f);
	transMatrix_sea = XMMatrixTranslation(0.0f, m_waterHeight, 0.0f);
	worldMatrix_sea = scaleMatrix_sea * transMatrix_sea;

	scaleMatrix_seafloor = XMMatrixScaling(2000.0f, 0.10f, 2000.0f);
	transMatrix_seafloor = XMMatrixTranslation(0.0f, -8.0f, 0.0f);
	rotateMatrix_seafloor = XMMatrixRotationY(0);
	worldMatrix_seafloor = scaleMatrix_seafloor * rotateMatrix_seafloor * transMatrix_seafloor;

	/// 지형지물 레벨디자인 행렬연산 ///

	scaleMatrix_island[0] = XMMatrixScaling(1.7f, 1.7f, 1.7f);
	transMatrix_island[0] = XMMatrixTranslation(-100.0f, 5.0f, -200.0f);
	rotateMatrix_island[0] = XMMatrixRotationY(0);

	scaleMatrix_island[1] = XMMatrixScaling(2.7, 1.3f, 2.7);
	transMatrix_island[1] = XMMatrixTranslation(-50.0f, 1.5f, -220.0f);
	rotateMatrix_island[1] = XMMatrixRotationY(0);

	scaleMatrix_island[2] = XMMatrixScaling(1.2f, 1.1f, 0.7f);
	transMatrix_island[2] = XMMatrixTranslation(-150.0f, 0.5f, -220.0f);
	rotateMatrix_island[2] = XMMatrixRotationY(0);

	scaleMatrix_island[3] = XMMatrixScaling(2.3f, 2.0f, 2.3f);
	transMatrix_island[3] = XMMatrixTranslation(150.0f, 1.5f, 50.0f);
	rotateMatrix_island[3] = XMMatrixRotationY(0);

	scaleMatrix_island[4] = XMMatrixScaling(1.0f, 0.8f, 1.0f);
	transMatrix_island[4] = XMMatrixTranslation(200.0f, 1.1f, 100.0f);
	rotateMatrix_island[4] = XMMatrixRotationY(0);

	scaleMatrix_island[5] = XMMatrixScaling(3.0f, 2.0f, 3.0f);
	transMatrix_island[5] = XMMatrixTranslation(-200.0f, 1.5f, 200.0f);
	rotateMatrix_island[5] = XMMatrixRotationY(0);

	scaleMatrix_island[6] = XMMatrixScaling(2.0f, 0.8f, 2.0f);
	transMatrix_island[6] = XMMatrixTranslation(-130.0f, 1.5f, 210.0f);
	rotateMatrix_island[6] = XMMatrixRotationY(0);

	scaleMatrix_island[7] = XMMatrixScaling(2.5f, 1.0f, 2.5f);
	transMatrix_island[7] = XMMatrixTranslation(-165.0f, 1.5f, 250.0f);
	rotateMatrix_island[7] = XMMatrixRotationY(0);

	scaleMatrix_island[8] = XMMatrixScaling(1.5f, 1.5f, 1.5f);
	transMatrix_island[8] = XMMatrixTranslation(0.0f, 1.0f, 50.0f);
	rotateMatrix_island[8] = XMMatrixRotationY(0.0f);

	num_island = 9;

	transMatrix_harbor[0] = XMMatrixTranslation(-160.0f, -0.5f, 345.0f);
	rotateMatrix_harbor[0] = XMMatrixRotationY(XMConvertToRadians(180));

	transMatrix_harbor[1] = XMMatrixTranslation(-166.0f, -0.5f, -170.0f);
	rotateMatrix_harbor[1] = XMMatrixRotationY(XMConvertToRadians(90));

	transMatrix_harbor[2] = XMMatrixTranslation(-126.0f, -0.5f, -122.0f);
	rotateMatrix_harbor[2] = XMMatrixRotationY(XMConvertToRadians(180));

	transMatrix_harbor[3] = XMMatrixTranslation(240.0f, -0.5f, 40.0f);
	rotateMatrix_harbor[3] = XMMatrixRotationY(XMConvertToRadians(-90));

	transMatrix_harbor[4] = XMMatrixTranslation(-80.0f, -0.5f, 290.0f);
	rotateMatrix_harbor[4] = XMMatrixRotationY(XMConvertToRadians(-90));

	num_harbor = 5;

	transMatrix_lHouse[0] = XMMatrixTranslation(-5.0f, 19.0f, 55.0f);
	rotateMatrix_lHouse[0] = XMMatrixRotationY(XMConvertToRadians(135));

	transMatrix_lHouse[1] = XMMatrixTranslation(-110.0f, 3, 315.0f);
	rotateMatrix_lHouse[1] = XMMatrixRotationY(XMConvertToRadians(-150));

	transMatrix_lHouse[2] = XMMatrixTranslation(199, 10, 100);
	rotateMatrix_lHouse[2] = XMMatrixRotationY(XMConvertToRadians(150));

	num_iHouse = 3;

	transMatrix_Rocks[0] = XMMatrixTranslation(-94, -8.5f, 380);
	rotateMatrix_Rocks[0] = XMMatrixRotationY(XMConvertToRadians(45));

	transMatrix_Rocks[1] = XMMatrixTranslation(140, -8.5f, 190);
	rotateMatrix_Rocks[1] = XMMatrixRotationY(XMConvertToRadians(0));

	transMatrix_Rocks[2] = XMMatrixTranslation(0, -8.5f, -39);
	rotateMatrix_Rocks[2] = XMMatrixRotationY(XMConvertToRadians(45));

	transMatrix_Rocks[3] = XMMatrixTranslation(-80, -8.5f, 96);
	rotateMatrix_Rocks[3] = XMMatrixRotationY(XMConvertToRadians(-90));

	transMatrix_Rocks[4] = XMMatrixTranslation(-194, -8.5f, -80);
	rotateMatrix_Rocks[4] = XMMatrixRotationY(XMConvertToRadians(45));

	num_rocks = 5;

	////////////////////////////////////////////////// Destructable Objects ////////////////////////////////////////////////// 
	transMatrix_convoy[0] = XMMatrixTranslation(-163, 0, 33);
	rotateMatrix_convoy[0] = XMMatrixRotationY(XMConvertToRadians(45));

	transMatrix_convoy[1] = XMMatrixTranslation(23, 0, 168);
	rotateMatrix_convoy[1] = XMMatrixRotationY(XMConvertToRadians(210));

	transMatrix_convoy[2] = XMMatrixTranslation(96, 0, -101);
	rotateMatrix_convoy[2] = XMMatrixRotationY(XMConvertToRadians(90));

	num_convoy = 3;

	transMatrix_coastGuardBase[0] = XMMatrixTranslation(107, 10, 105);
	rotateMatrix_coastGuardBase[0] = XMMatrixRotationY(XMConvertToRadians(0));

	transMatrix_coastGuardBase[1] = XMMatrixTranslation(-103, 12, 219);
	rotateMatrix_coastGuardBase[1] = XMMatrixRotationY(XMConvertToRadians(0));

	transMatrix_coastGuardBase[2] = XMMatrixTranslation(16, 18, 57);
	rotateMatrix_coastGuardBase[2] = XMMatrixRotationY(XMConvertToRadians(0));

	num_coastGuardBase = 3;

	transMatrix_coastGuardCannon[0] = XMMatrixTranslation(107, 10, 105);
	rotateMatrix_coastGuardCannon[0] = XMMatrixRotationY(XMConvertToRadians(0));

	transMatrix_coastGuardCannon[1] = XMMatrixTranslation(-103, 12, 219);
	rotateMatrix_coastGuardCannon[1] = XMMatrixRotationY(XMConvertToRadians(0));

	transMatrix_coastGuardCannon[2] = XMMatrixTranslation(16, 18, 57);
	rotateMatrix_coastGuardCannon[2] = XMMatrixRotationY(XMConvertToRadians(0));

	num_coastGuardCannon = 3;

	rotateMatrix_fire = XMMatrixRotationY(camYaw + XMConvertToRadians(180));
	rotateMatrix_convoyfire = XMMatrixRotationY(camYaw + XMConvertToRadians(180));

	rotateMatrix_baseSmoke = XMMatrixRotationY(camYaw + XMConvertToRadians(180));
	rotateMatrix_convoySmoke = XMMatrixRotationY(camYaw + XMConvertToRadians(180));
	////////////////////////////////////////////////// Destructable Objects ////////////////////////////////////////////////// 

	for (int i = 0; i < 3; i++) {
		worldMatrix_lHouse[i] = scaleMatrix_lHouse * rotateMatrix_lHouse[i] * transMatrix_lHouse[i];
		worldMatrix_convoy[i] = scaleMatrix_convoy * rotateMatrix_convoy[i] * transMatrix_convoy[i];
		worldMatrix_coastGuardBase[i] = scaleMatrix_coastGuardBase * transMatrix_coastGuardBase[i];
		worldMatrix_coastGuardCannon[i] = scaleMatrix_coastGuardCannon * transMatrix_coastGuardCannon[i];

		worldMatrix_fire[i] = scaleMatrix_fire * rotateMatrix_fire * transMatrix_coastGuardBase[i] * transMatrix_fire;
		worldMatrix_convoyfire[i] = scaleMatrix_convoyfire * rotateMatrix_convoyfire * transMatrix_convoy[i] * transMatrix_convoyfire;

		worldMatrix_baseSmoke[i] = scaleMatrix_Smoke * rotateMatrix_baseSmoke * transMatrix_coastGuardBase[i];
		worldMatrix_convoySmoke[i] = scaleMatrix_Smoke * rotateMatrix_convoySmoke * transMatrix_convoy[i];

		m_Convoy[i]->SetCollider(scaleMatrix_convoy, transMatrix_convoy[i]);//colliderSet
		m_CoastGuardCannon[i]->SetCollider(scaleMatrix_coastGuardCannon, transMatrix_coastGuardCannon[i]);//colliderSet
	}
	for (int i = 0; i < 5; i++) {
		worldMatrix_Rocks[i] = scaleMatrix_Rocks * rotateMatrix_Rocks[i] * transMatrix_Rocks[i];
	}

	////////////////////////////////////////////////////////////////////////////////
	// UI 배치 시작하는 부분.
	////////////////////////////////////////////////////////////////////////////////

	XMMATRIX charCamHeightMat = XMMatrixTranslation(0.0f, charCamHeight, 0.0f);

	rotateMatrix_Text = XMMatrixRotationY(XMConvertToRadians(180));
	transMatrix_Text = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	worldMatrix_Text = rotateMatrix_Text * transMatrix_Text;

	rotateMatrix_UIBG = XMMatrixRotationRollPitchYaw(-camPitch, camYaw + XMConvertToRadians(180), 0);
	worldMatrix_UIBG = rotateMatrix_UIBG * transMatrix_Player * charCamHeightMat;
	worldMatrix_Gage = rotateMatrix_UIBG * transMatrix_Player;


	rotateMatrix_CH = XMMatrixRotationRollPitchYaw(-camPitch, camYaw + XMConvertToRadians(180), XMConvertToRadians(45));
	worldMatrix_CH = rotateMatrix_CH * transMatrix_Player * charCamHeightMat;

	rotateMatrix_CHA = XMMatrixRotationRollPitchYaw(-camPitch, camYaw + XMConvertToRadians(180), XMConvertToRadians(45));
	worldMatrix_CHA = rotateMatrix_CHA * transMatrix_Player * charCamHeightMat;

	rotateMatrix_shipUIr = XMMatrixRotationRollPitchYaw(0, 0, -playerYaw);
	rotateMatrix_shipUI = XMMatrixRotationRollPitchYaw(-camPitch, camYaw + XMConvertToRadians(180), 0);
	transMatrix_shipUI = XMMatrixTranslation((screenwidth / 2) - 50, -(screenheight / 2) + 50, 0.0f);
	transMatrix_shipUIp = XMMatrixTranslation(-(screenwidth / 2) + 105, -(screenheight / 2) + 125, 0.0f);

	worldMatrix_shipUI = transMatrix_shipUI * rotateMatrix_shipUIr * transMatrix_shipUIp * rotateMatrix_shipUI * transMatrix_Player * charCamHeightMat;

	rotateMatrix_TurnBar = XMMatrixRotationRollPitchYaw(-camPitch, camYaw + XMConvertToRadians(180), 0);
	transMatrix_TurnBarC = XMMatrixTranslation((screenwidth / 2) - 12, -screenheight + 250, 0.0f);
	transMatrix_TurnBar = XMMatrixTranslation(turnSpeed*200000.0f, 0.0f, 0.0f);

	worldMatrix_TurnBar = transMatrix_TurnBarC * transMatrix_TurnBar * rotateMatrix_TurnBar * transMatrix_Player * charCamHeightMat;

	for (int i = 0; i < 2; i++) {
		rotateMatrix_CH_arm[i] = XMMatrixRotationRollPitchYaw(-camPitch, camYaw + XMConvertToRadians(180), 0);
		worldMatrix_CH_arm[i] = rotateMatrix_CH_arm[i] * transMatrix_Player * charCamHeightMat;

		rotateMatrix_CHA_arm[i] = XMMatrixRotationRollPitchYaw(-camPitch, camYaw + XMConvertToRadians(180), 0);
		worldMatrix_CHA_arm[i] = rotateMatrix_CHA_arm[i] * transMatrix_Player * charCamHeightMat;
	}

	////////////////////////////////////////////////////////////////////////////////
	// 파티클 배치 시작하는 부분.
	////////////////////////////////////////////////////////////////////////////////

	transMatrix_Smoke = XMMatrixTranslation(0.0f, 4.0f, 3.5f);

	rotateMatrix_Smoke = XMMatrixRotationRollPitchYaw(0, XMConvertToRadians(-90), 0);
	worldMatrix_Smoke = rotateMatrix_Smoke * transMatrix_Smoke * rotateMatrix_Player * transMatrix_Player;

	rotateMatrix_Smoke_2 = XMMatrixRotationRollPitchYaw(0, camYaw + XMConvertToRadians(-180), 0);
	rotateMatrix_Smoke_3 = XMMatrixRotationRollPitchYaw(0, camYaw + XMConvertToRadians(-180), 0);

	if (wayPointNum != 0) {
		if (collisionWithContainer) {
			if (spacePressTime > 3.0f) { //스페이스바 홀딩
				containerNum--;
				wayPointNum--;
				spacePressTime = 0.0f;
			}
		}
		if (collisionWithFlag) {
			flagNum--;
			wayPointNum--;
		}
	}

	transMatrix_flagContainer = XMMatrixTranslation(0.0f, m_playerPosition.y * 2.0f + 0.5f, 0.0f);
	rotateMatrix_flagContainer = XMMatrixRotationRollPitchYaw(playerPitch * 20.0f, rotate_Entity, playerPitch * 20.0f);

	rotate_Entity += 0.01f * m_fps_Calculate;

	transMatrix_Smoke_2 = XMMatrixTranslation(transFlagX[8 - wayPointNum], 0.0f, transFlagZ[8 - wayPointNum]);
	worldMatrix_Smoke_2 = rotateMatrix_Smoke_2 * transMatrix_Smoke_2;
	transMatrix_Smoke_3 = XMMatrixTranslation(transContainX[8 - wayPointNum], 0.0f, transContainZ[8 - wayPointNum]);
	worldMatrix_Smoke_3 = rotateMatrix_Smoke_3 * transMatrix_Smoke_3;

	scaleMatrix_flag = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	worldMatrix_flag = scaleMatrix_flag * rotateMatrix_flagContainer * transMatrix_Smoke_2 * transMatrix_flagContainer;
	m_Flag->SetCollider(scaleMatrix_flag_for_Collision, transMatrix_Smoke_2);//colliderSet

	scaleMatrix_container = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	worldMatrix_container = scaleMatrix_container * rotateMatrix_flagContainer * transMatrix_Smoke_3 * transMatrix_flagContainer;
	m_Container->SetCollider(scaleMatrix_container_for_Collision, transMatrix_Smoke_3);//colliderSet
}

void GraphicsClass::RestartScene() {
	collisionFront = false;
	collisionBack = false;
	frameTime = 0.0f;
	reloadTime = 0;
	shooting = false;

	m_waterHeight = 0;
	m_waterTranslation = 0;
	turnSpeed = 0.0f;

	charCamDist = 60.0f;
	charCamHeight = 12.0f;
	CamHeight = 0.0f;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;
	currentSpeedState = 0;

	camRotateInfoYaw = 0.0f;
	camRotateInfoPitch = 0.0f;

	playerYaw = 0.0f;
	playerPitch = 0.0f;
	playerRoll = 0.0f;

	camYaw = 0.0f;
	camPitch = 0.0f;

	spacePressTime = 0.0f;
	m_time = 0.0f;

	num_island = 0;
	num_harbor = 0;

	clear = false;
	fail = false;

	isAssend = false;
	isTilt = false;

	isRetryPressed = false;

	flagNum = 4;
	containerNum = 3;

	wayPointNum = 8;

	pitch = 0;
	speed = 0;

	screenwidth;
	screenheight;

	m_mouseX = 0;
	m_mouseY = 0;

	num_total = 0;

	accelate_Assend = 0.000f;
	accelate_Tilt = 0.0f;
	turretRad = 0.0f;

	desiredCharDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	playerPosition = XMVectorSet(0.0f, 0.2f, 700.0f, 0.0f);

	playerPosition4Cam = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	crossForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	crossRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	Cross_Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	Dot = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	CamLookDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	m_playerPosition;

	XMStoreFloat3(&m_playerPosition, playerPosition);

	DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	CamDefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	CamDefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	playerTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	playerUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	playerForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	playerRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 3; i++) {
		m_Bullet[i]->ProjReload(playerPosition + Vector_Turret[i]);

		convoyHit[i] = false;
		coastGuardCannonHit[i] = false;
	}
}

bool GraphicsClass::Render()
{
	bool result;

	SetPosition();
	
	if (isMaintitle) {
		result = RenderMainTitleScene();
		RestartScene();
	}

	if (isGameScene) {
		result = RenderTutorialScene();
	}

	if (isResultScene) {
		result = RenderResultScene();
	}

	return true;
}

bool GraphicsClass::RenderMainTitleScene()
{
	bool result;
	/// <summary>
	/// 메인타이틀 씬, 조작법, 개발자 정보, 게임 목표 등등. 튜토리얼 씬으로 가거나, 게임 종료.
	/// </summary>
	/// <returns></returns>
	XMMATRIX viewMatrix, projectionMatrix, reflectionMatrix, orthoMatrix;

	m_D3D->BeginScene(1.0f, 1.0f, 1.0f, 1.0f);
	playerPosition4Cam = playerPosition;
	playerPosition4Cam = XMVectorSetY(playerPosition4Cam, 0.0f);
	m_Camera->Render(camYaw, camPitch, moveLeftRight, moveBackForward, CamDefaultForward, CamDefaultRight, playerPosition4Cam, charCamDist, charCamHeight);

	m_Camera->GetViewMatrix(viewMatrix);
	m_Camera->GetReflectionViewMatrix(reflectionMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	m_D3D->TurnOnAlphaBlending();
	m_D3D->TurnZBufferOff();

	m_UIBGforTitle->Render(m_D3D->GetDeviceContext(), 0, 0);

	m_TextureBlendShader->Render(m_D3D->GetDeviceContext(), m_UIBGforTitle->GetIndexCount(), worldMatrix_UIBG,
		viewMatrix, orthoMatrix, m_UIBGforTitle->GetTexture());

	if (isMenuPressed) {
		m_Menu->Render(m_D3D->GetDeviceContext(), (screenwidth / 2) - 200, (screenheight / 2) - 300);

		m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Menu->GetIndexCount(), worldMatrix_UIBG,
			viewMatrix, orthoMatrix, m_Menu->GetTexture());
	}
	else {

		m_MainTitle->Render(m_D3D->GetDeviceContext(), (screenwidth / 2) - ((screenheight * 1.25f) / 2), 0);

		m_TextureShader->Render(m_D3D->GetDeviceContext(), m_MainTitle->GetIndexCount(), worldMatrix_UIBG,
			viewMatrix, orthoMatrix, m_MainTitle->GetTexture());
	}

	

	m_D3D->TurnZBufferOn();
	m_D3D->TurnOffAlphaBlending();

	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::RenderResultScene()
{
	bool result;
	/// <summary>
	/// 결과창, 성공/실패여부, 성공했다면 클리어 시간 출력. 무조건 메인타이틀로 돌아감.
	/// </summary>
	/// <returns></returns>

	XMMATRIX viewMatrix, projectionMatrix, reflectionMatrix, orthoMatrix;

	m_D3D->BeginScene(1.0f, 1.0f, 1.0f, 1.0f);
	playerPosition4Cam = playerPosition;
	playerPosition4Cam = XMVectorSetY(playerPosition4Cam, 0.0f);
	m_Camera->Render(camYaw, camPitch, moveLeftRight, moveBackForward, CamDefaultForward, CamDefaultRight, playerPosition4Cam, charCamDist, charCamHeight);

	m_Camera->GetViewMatrix(viewMatrix);
	m_Camera->GetReflectionViewMatrix(reflectionMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	m_D3D->TurnOnAlphaBlending();
	m_D3D->TurnZBufferOff();

	m_UIBGforTitle->Render(m_D3D->GetDeviceContext(), 0, 0);

	m_TextureBlendShader->Render(m_D3D->GetDeviceContext(), m_UIBGforTitle->GetIndexCount(), worldMatrix_UIBG,
		viewMatrix, orthoMatrix, m_UIBGforTitle->GetTexture());

	m_ResultScene->Render(m_D3D->GetDeviceContext(), (screenwidth / 2) - ((screenheight * 1.25f) / 2), 0);

	m_TextureShader->Render(m_D3D->GetDeviceContext(), m_ResultScene->GetIndexCount(), worldMatrix_UIBG,
		viewMatrix, orthoMatrix, m_ResultScene->GetTexture());

	// 텍스트 출력부분
	result = m_Text->RenderResult(m_D3D->GetDeviceContext(), worldMatrix_Text, orthoMatrix);

	m_D3D->TurnZBufferOn();
	m_D3D->TurnOffAlphaBlending();

	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::RenderTutorialScene()
{
	bool result;
	/// <summary>
	/// 튜토리얼 게임 씬, R을 눌러 언제든 재시작 가능, ESC를 눌러 메뉴 호출(종료, 메인타이틀로 돌아가기).
	/// </summary>
	/// <returns></returns>

	if (isRetryPressed == true) {
		RestartScene();
	}

	result = RenderTutorialGameReflectionToTexture();
	result = RenderTutorialGameRefractionToTexture();
	result = RenderTutorialGameScene();

	return true;
}


bool GraphicsClass::RenderTutorialGameRefractionToTexture()
{
	XMFLOAT4 clipPlane;
	XMMATRIX viewMatrix, projectionMatrix;
	bool result;

	// Setup a clipping plane based on the height of the water to clip everything above it.
	clipPlane = XMFLOAT4(0.0f, -1.0f, 0.0f, m_waterHeight + 0.1f);

	// Set the render target to be the refraction render to texture.
	m_RefractionTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// Clear the refraction render to texture.
	m_RefractionTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_D3D->TurnOffCulling();
	m_D3D->TurnDSLessEqualOn();

	CamHeight = m_Camera->GetPosition().y;

	m_SkyMap->Render(m_D3D->GetDeviceContext(), worldMatrix_skymap, viewMatrix, projectionMatrix);

	m_D3D->TurnDSLessEqualOff();
	m_D3D->TurnOnCulling();

	m_Turret->Render(m_D3D->GetDeviceContext());

	for (int i = 0; i < 3; i++) {
		// Render the model using the light shader.
		result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Turret->GetIndexCount(), worldMatrix_Turret[i], viewMatrix,
			projectionMatrix, m_Turret->GetTexture(), m_Light->GetDirection(),
			WaterAmbientColor, WaterDiffuseColor, clipPlane, rotateMatrix_Turret[i]);
	}

	// Put the bath model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Seafloor->Render(m_D3D->GetDeviceContext());

	result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Seafloor->GetIndexCount(), worldMatrix_seafloor, viewMatrix,
		projectionMatrix, m_Seafloor->GetTexture(), m_Light->GetDirection(),
		WaterAmbientColor, WaterDiffuseColor, clipPlane, rotateMatrix_seafloor);

	m_Island->Render(m_D3D->GetDeviceContext());

	for (int i = 0; i < 9; i++) {
		result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Island->GetIndexCount(), worldMatrix_island[i], viewMatrix,
			projectionMatrix, m_Island->GetTexture(), m_Light->GetDirection(),
			WaterAmbientColor, WaterDiffuseColor, clipPlane, rotateMatrix_island[i]);
	}

	m_Harbor->Render(m_D3D->GetDeviceContext());

	for (int i = 0; i < 5; i++) {
		// Render the model using the light shader.
		result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Harbor->GetIndexCount(), worldMatrix_harbor[i], viewMatrix,
			projectionMatrix, m_Harbor->GetTexture(), m_Light->GetDirection(),
			WaterAmbientColor, WaterDiffuseColor, clipPlane, rotateMatrix_harbor[i]);
	}

	m_Rocks->Render(m_D3D->GetDeviceContext());

	for (int i = 0; i < 5; i++) {
		// Render the model using the light shader.
		result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Rocks->GetIndexCount(), worldMatrix_Rocks[i], viewMatrix, projectionMatrix,
			m_Rocks->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane, rotateMatrix_Rocks[i]);
	}

	m_Player->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Player->GetIndexCount(), worldMatrix_Player, viewMatrix,
		projectionMatrix, m_Player->GetTexture(), m_Light->GetDirection(),
		WaterAmbientColor, WaterDiffuseColor, clipPlane, rotateMatrix_Player);

	m_Flag->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Flag->GetIndexCount(), worldMatrix_flag, viewMatrix, projectionMatrix,
		m_Flag->GetTexture(), m_Light->GetDirection(), WaterAmbientColor, WaterDiffuseColor,
		clipPlane, rotateMatrix_flagContainer);

	m_Container->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Container->GetIndexCount(), worldMatrix_container, viewMatrix, projectionMatrix,
		m_Container->GetTexture(), m_Light->GetDirection(), WaterAmbientColor, WaterDiffuseColor,
		clipPlane, rotateMatrix_flagContainer);

	for (int i = 0; i < 3; i++) {
		m_Bullet[i]->Render(m_D3D->GetDeviceContext());

		result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Bullet[i]->GetIndexCount(), worldMatrix_Bullet[i], viewMatrix, projectionMatrix,
			m_Bullet[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			clipPlane, rotateMatrix_flagContainer);
	}

	m_CoastGuardBase->Render(m_D3D->GetDeviceContext());

	for (int i = 0; i < 3; i++) {
		// Render the model using the light shader.
		result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_CoastGuardBase->GetIndexCount(), worldMatrix_coastGuardBase[i], viewMatrix, projectionMatrix,
			m_CoastGuardBase->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			clipPlane, rotateMatrix_coastGuardBase[i]);
	}

	for (int i = 0; i < 3; i++) {
		m_Convoy[i]->Render(m_D3D->GetDeviceContext());
		// Render the model using the light shader.
		result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Convoy[i]->GetIndexCount(), worldMatrix_convoy[i], viewMatrix, projectionMatrix,
			m_Convoy[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			clipPlane, rotateMatrix_convoy[i]);

		if (!coastGuardCannonHit[i])
		{
			m_CoastGuardCannon[i]->Render(m_D3D->GetDeviceContext());
			// Render the model using the light shader.
			result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_CoastGuardCannon[i]->GetIndexCount(), worldMatrix_coastGuardCannon[i], viewMatrix, projectionMatrix,
				m_CoastGuardCannon[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				clipPlane, rotateMatrix_coastGuardCannon[i]);
		}
	}

	m_D3D->TurnOffCulling();
	m_D3D->TurnOnAlphaBlending();

	static float fireTime = 0.0f;


	// 프레임 시간 카운터를 증가시킵니다.
	fireTime += 0.01f;
	if (fireTime > 1000.0f)
	{
		fireTime = 0.0f;
	}

	// 세 가지 다른 노이즈 텍스처에 대해 세 가지 스크롤 속도를 설정합니다.
	XMFLOAT3 scrollSpeeds = XMFLOAT3(1.3f, 2.1f, 2.3f);

	// 세 개의 서로 다른 노이즈 옥타브 텍스처를 만드는 데 사용할 세 개의 스케일을 설정합니다.
	XMFLOAT3 scales = XMFLOAT3(2.0f, 3.0f, 4.0f);

	// 세 가지 다른 노이즈 텍스처에 대해 세 가지 다른 x 및 y 왜곡 인수를 설정합니다.
	XMFLOAT2 distortion1 = XMFLOAT2(0.1f, 0.2f);
	XMFLOAT2 distortion2 = XMFLOAT2(0.1f, 0.3f);
	XMFLOAT2 distortion3 = XMFLOAT2(0.1f, 0.1f);

	// 텍스처 좌표 샘플링 섭동의 스케일과 바이어스.
	float distortionScale = 2.5f;
	float distortionBias = 1.5f;

	// 정사각형 모델의 정점과 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
	for (int i = 0; i < 3; i++) {
		m_Fire[i]->Render(m_D3D->GetDeviceContext());
		if (coastGuardCannonHit[i]) {
			// 화재 쉐이더를 사용하여 사각형 모델을 렌더링합니다.
			if (!m_FireShader->Render(m_D3D->GetDeviceContext(), m_Fire[i]->GetIndexCount(), worldMatrix_fire[i], viewMatrix, projectionMatrix,
				m_Fire[i]->GetTexture1(), m_Fire[i]->GetTexture2(), m_Fire[i]->GetTexture3(), fireTime, scrollSpeeds,
				scales, distortion1, distortion2, distortion3, distortionScale, distortionBias))
			{
				return false;
			}
		}

		m_convoyFire[i]->Render(m_D3D->GetDeviceContext());
		if (convoyHit[i]) {
			// 화재 쉐이더를 사용하여 사각형 모델을 렌더링합니다.
			if (!m_FireShader->Render(m_D3D->GetDeviceContext(), m_convoyFire[i]->GetIndexCount(), worldMatrix_convoyfire[i], viewMatrix, projectionMatrix,
				m_convoyFire[i]->GetTexture1(), m_convoyFire[i]->GetTexture2(), m_convoyFire[i]->GetTexture3(), fireTime, scrollSpeeds,
				scales, distortion1, distortion2, distortion3, distortionScale, distortionBias))
			{
				return false;
			}
		}
	}

	m_Smoke_2->Render(m_D3D->GetDeviceContext());
	if (!m_SmokeShaderClass->Render(m_D3D->GetDeviceContext(), m_Smoke_2->GetIndexCount(), worldMatrix_Smoke_2, viewMatrix, projectionMatrix,
		m_Smoke_2->GetTexture()))
	{
		return false;
	}

	m_Smoke_3->Render(m_D3D->GetDeviceContext());
	if (!m_SmokeShaderClass->Render(m_D3D->GetDeviceContext(), m_Smoke_3->GetIndexCount(), worldMatrix_Smoke_3, viewMatrix, projectionMatrix,
		m_Smoke_3->GetTexture()))
	{
		return false;
	}

	for (int i = 0; i < 3; i++) {
		m_baseSmoke[i]->Render(m_D3D->GetDeviceContext());
		if (coastGuardCannonHit[i]) {
			if (!m_SmokeShaderClass->Render(m_D3D->GetDeviceContext(), m_baseSmoke[i]->GetIndexCount(), worldMatrix_baseSmoke[i], viewMatrix, projectionMatrix,
				m_baseSmoke[i]->GetTexture()))
			{
				return false;
			}
		}

		m_convoySmoke[i]->Render(m_D3D->GetDeviceContext());
		if (convoyHit[i]) {
			if (!m_SmokeShaderClass->Render(m_D3D->GetDeviceContext(), m_convoySmoke[i]->GetIndexCount(), worldMatrix_convoySmoke[i], viewMatrix, projectionMatrix,
				m_convoySmoke[i]->GetTexture()))
			{
				return false;
			}
		}
	}

	m_D3D->TurnZBufferOff();
	m_Smoke_1->Render(m_D3D->GetDeviceContext());
	if (!m_SmokeShaderClass->Render(m_D3D->GetDeviceContext(), m_Smoke_1->GetIndexCount(), worldMatrix_Smoke, viewMatrix, projectionMatrix,
		m_Smoke_1->GetTexture()))
	{
		return false;
	}
	m_D3D->TurnOnCulling();
	m_D3D->TurnZBufferOn();
	m_D3D->TurnOffAlphaBlending();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	return true;
}


bool GraphicsClass::RenderTutorialGameReflectionToTexture()
{
	XMFLOAT4 clipPlane;
	XMMATRIX reflectionViewMatrix, projectionMatrix;
	bool result;

	clipPlane = XMFLOAT4(0.0f, 1.0f, 0.0f, m_waterHeight + 0.1f);

	// Set the render target to be the reflection render to texture.
	m_ReflectionTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// Clear the reflection render to texture.
	m_ReflectionTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Use the camera to render the reflection and create a reflection view matrix.
	m_Camera->RenderReflection(camYaw, camPitch, moveLeftRight, moveBackForward, CamDefaultForward, CamDefaultRight,
		                       playerPosition4Cam, charCamDist, charCamHeight, m_waterHeight);
	CamLookDir = m_Camera->GetLookDir();
	// Get the camera reflection view matrix instead of the normal view matrix.
	m_Camera->GetReflectionViewMatrix(reflectionViewMatrix);

	// Get the world and projection matrices from the d3d object.
	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_D3D->TurnOffCulling();
	m_D3D->TurnDSLessEqualOn();

	m_SkyMap->Render(m_D3D->GetDeviceContext(), worldMatrix_skymap, reflectionViewMatrix, projectionMatrix);

	m_D3D->TurnDSLessEqualOff();
	m_D3D->TurnOnCulling();

	m_Turret->Render(m_D3D->GetDeviceContext());

	for (int i = 0; i < 3; i++) {
		// Render the model using the light shader.
		result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Turret->GetIndexCount(), worldMatrix_Turret[i], reflectionViewMatrix,
			projectionMatrix, m_Turret->GetTexture(), m_Light->GetDirection(),
			m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane, rotateMatrix_Turret[i]);
	}

	m_Island->Render(m_D3D->GetDeviceContext());

	for (int i = 0; i < 9; i++) {
		result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Island->GetIndexCount(), worldMatrix_island[i], reflectionViewMatrix,
			projectionMatrix, m_Island->GetTexture(), m_Light->GetDirection(),
			m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane, rotateMatrix_island[i]);
	}

	m_Harbor->Render(m_D3D->GetDeviceContext());

	for (int i = 0; i < 5; i++) {
		// Render the model using the light shader.
		result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Harbor->GetIndexCount(), worldMatrix_harbor[i], reflectionViewMatrix, projectionMatrix,
			m_Harbor->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane, rotateMatrix_harbor[i]);
	}

	m_lHouse->Render(m_D3D->GetDeviceContext());

	for (int i = 0; i < 3; i++) {
		// Render the model using the light shader.
		result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Harbor->GetIndexCount(), worldMatrix_lHouse[i], reflectionViewMatrix, projectionMatrix,
			m_Harbor->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane, rotateMatrix_lHouse[i]);
	}

	m_Rocks->Render(m_D3D->GetDeviceContext());

	for (int i = 0; i < 5; i++) {
		// Render the model using the light shader.
		result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Rocks->GetIndexCount(), worldMatrix_Rocks[i], reflectionViewMatrix, projectionMatrix,
			m_Rocks->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane, rotateMatrix_Rocks[i]);
	}

	m_Player->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Player->GetIndexCount(), worldMatrix_Player, reflectionViewMatrix,
		projectionMatrix, m_Player->GetTexture(), m_Light->GetDirection(),
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane, rotateMatrix_Player);

	m_Flag->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Flag->GetIndexCount(), worldMatrix_flag, reflectionViewMatrix, projectionMatrix,
		m_Flag->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		 clipPlane, rotateMatrix_flagContainer);

	m_Container->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Container->GetIndexCount(), worldMatrix_container, reflectionViewMatrix, projectionMatrix,
		m_Container->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		clipPlane, rotateMatrix_flagContainer);

	m_CoastGuardBase->Render(m_D3D->GetDeviceContext());

	for (int i = 0; i < 3; i++) {
		// Render the model using the light shader.
		result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_CoastGuardBase->GetIndexCount(), worldMatrix_coastGuardBase[i], reflectionViewMatrix, projectionMatrix,
			m_CoastGuardBase->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			clipPlane, rotateMatrix_coastGuardBase[i]);
	}

	for (int i = 0; i < 3; i++) {
		m_Bullet[i]->Render(m_D3D->GetDeviceContext());

		result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Bullet[i]->GetIndexCount(), worldMatrix_Bullet[i], reflectionViewMatrix, projectionMatrix,
			m_Bullet[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			clipPlane, rotateMatrix_flagContainer);
	}

	for (int i = 0; i < 3; i++) {
		m_Convoy[i]->Render(m_D3D->GetDeviceContext());
		// Render the model using the light shader.
		result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Convoy[i]->GetIndexCount(), worldMatrix_convoy[i], reflectionViewMatrix, projectionMatrix,
			m_Convoy[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			clipPlane, rotateMatrix_convoy[i]);

		if (!coastGuardCannonHit[i])
		{
			m_CoastGuardCannon[i]->Render(m_D3D->GetDeviceContext());
			// Render the model using the light shader.
			result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_CoastGuardCannon[i]->GetIndexCount(), worldMatrix_coastGuardCannon[i], reflectionViewMatrix, projectionMatrix,
				m_CoastGuardCannon[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				clipPlane, rotateMatrix_coastGuardCannon[i]);
		}
	}

	m_D3D->TurnOffCulling();
	m_D3D->TurnOnAlphaBlending();

	static float fireTime = 0.0f;


	// 프레임 시간 카운터를 증가시킵니다.
	fireTime += 0.01f;
	if (fireTime > 1000.0f)
	{
		fireTime = 0.0f;
	}

	// 세 가지 다른 노이즈 텍스처에 대해 세 가지 스크롤 속도를 설정합니다.
	XMFLOAT3 scrollSpeeds = XMFLOAT3(1.3f, 2.1f, 2.3f);

	// 세 개의 서로 다른 노이즈 옥타브 텍스처를 만드는 데 사용할 세 개의 스케일을 설정합니다.
	XMFLOAT3 scales = XMFLOAT3(2.0f, 3.0f, 4.0f);

	// 세 가지 다른 노이즈 텍스처에 대해 세 가지 다른 x 및 y 왜곡 인수를 설정합니다.
	XMFLOAT2 distortion1 = XMFLOAT2(0.1f, 0.2f);
	XMFLOAT2 distortion2 = XMFLOAT2(0.1f, 0.3f);
	XMFLOAT2 distortion3 = XMFLOAT2(0.1f, 0.1f);

	// 텍스처 좌표 샘플링 섭동의 스케일과 바이어스.
	float distortionScale = 2.5f;
	float distortionBias = 1.5f;

	// 정사각형 모델의 정점과 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
	for (int i = 0; i < 3; i++) {
		m_Fire[i]->Render(m_D3D->GetDeviceContext());
		if (coastGuardCannonHit[i]) {
			// 화재 쉐이더를 사용하여 사각형 모델을 렌더링합니다.
			if (!m_FireShader->Render(m_D3D->GetDeviceContext(), m_Fire[i]->GetIndexCount(), worldMatrix_fire[i], reflectionViewMatrix, projectionMatrix,
				m_Fire[i]->GetTexture1(), m_Fire[i]->GetTexture2(), m_Fire[i]->GetTexture3(), fireTime, scrollSpeeds,
				scales, distortion1, distortion2, distortion3, distortionScale, distortionBias))
			{
				return false;
			}
		}

		m_convoyFire[i]->Render(m_D3D->GetDeviceContext());
		if (convoyHit[i]) {
			// 화재 쉐이더를 사용하여 사각형 모델을 렌더링합니다.
			if (!m_FireShader->Render(m_D3D->GetDeviceContext(), m_convoyFire[i]->GetIndexCount(), worldMatrix_convoyfire[i], reflectionViewMatrix, projectionMatrix,
				m_convoyFire[i]->GetTexture1(), m_convoyFire[i]->GetTexture2(), m_convoyFire[i]->GetTexture3(), fireTime, scrollSpeeds,
				scales, distortion1, distortion2, distortion3, distortionScale, distortionBias))
			{
				return false;
			}
		}
	}

	m_Smoke_2->Render(m_D3D->GetDeviceContext());
	if (!m_SmokeShaderClass->Render(m_D3D->GetDeviceContext(), m_Smoke_2->GetIndexCount(), worldMatrix_Smoke_2, reflectionViewMatrix, projectionMatrix,
		m_Smoke_2->GetTexture()))
	{
		return false;
	}

	m_Smoke_3->Render(m_D3D->GetDeviceContext());
	if (!m_SmokeShaderClass->Render(m_D3D->GetDeviceContext(), m_Smoke_3->GetIndexCount(), worldMatrix_Smoke_3, reflectionViewMatrix, projectionMatrix,
		m_Smoke_3->GetTexture()))
	{
		return false;
	}

	for (int i = 0; i < 3; i++) {
		m_baseSmoke[i]->Render(m_D3D->GetDeviceContext());
		if (coastGuardCannonHit[i]) {
			if (!m_SmokeShaderClass->Render(m_D3D->GetDeviceContext(), m_baseSmoke[i]->GetIndexCount(), worldMatrix_baseSmoke[i], reflectionViewMatrix, projectionMatrix,
				m_baseSmoke[i]->GetTexture()))
			{
				return false;
			}
		}

		m_convoySmoke[i]->Render(m_D3D->GetDeviceContext());
		if (convoyHit[i]) {
			if (!m_SmokeShaderClass->Render(m_D3D->GetDeviceContext(), m_convoySmoke[i]->GetIndexCount(), worldMatrix_convoySmoke[i], reflectionViewMatrix, projectionMatrix,
				m_convoySmoke[i]->GetTexture()))
			{
				return false;
			}
		}
	}

	m_D3D->TurnZBufferOff();
	m_Smoke_1->Render(m_D3D->GetDeviceContext());
	if (!m_SmokeShaderClass->Render(m_D3D->GetDeviceContext(), m_Smoke_1->GetIndexCount(), worldMatrix_Smoke, reflectionViewMatrix, projectionMatrix,
		m_Smoke_1->GetTexture()))
	{
		return false;
	}
	m_D3D->TurnOnCulling();
	m_D3D->TurnZBufferOn();
	m_D3D->TurnOffAlphaBlending();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	return true;
}


bool GraphicsClass::RenderTutorialGameScene()
{
	m_indexCnt = m_Island->getPolygon() * num_island
		+ m_Player->getPolygon() + m_Seafloor->getPolygon() + m_Sea->getPolygon() + m_Turret->getPolygon() *
		num_turret + m_Harbor->getPolygon() * num_harbor + m_lHouse->getPolygon() * num_iHouse
		+ m_Rocks->getPolygon() * num_rocks;

	num_total = 4 + num_island + num_turret + num_harbor + num_iHouse;

	XMMATRIX viewMatrix, projectionMatrix, reflectionMatrix, orthoMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	playerPosition4Cam = playerPosition;
	playerPosition4Cam = XMVectorSetY(playerPosition4Cam, 0.0f);
	// Generate the view matrix based on the camera's position.
	m_Camera->Render(camYaw, camPitch, moveLeftRight, moveBackForward, CamDefaultForward, CamDefaultRight, playerPosition4Cam, charCamDist, charCamHeight);
	CamLookDir = m_Camera->GetLookDir();

	// Get the world, view, and projection matrices from the camera and d3d objects.

	/*m_D3D->GetWorldMatrix(worldMatrix_Text);
	m_D3D->GetWorldMatrix(worldMatrix_Bitmap);*/
	m_Camera->GetViewMatrix(viewMatrix);
	m_Camera->GetReflectionViewMatrix(reflectionMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	m_D3D->TurnOffCulling();
	m_D3D->TurnDSLessEqualOn();

	m_SkyMap->Render(m_D3D->GetDeviceContext(), worldMatrix_skymap, viewMatrix, projectionMatrix);

	m_D3D->TurnDSLessEqualOff();
	m_D3D->TurnOnCulling();

	isBoundary = !m_Player->checkCollider(1000.0f, -1000.0f, 1000.0f, -1000.0f, 1000.0f, -1000.0f);

	if (isBoundary) {
		if (currentSpeedState > 0) {
			collisionFront = true;
		}
		else if (currentSpeedState < 0) {
			collisionBack = true;
		}
	}

	for (int i = 0; i < 9; i++) {
		worldMatrix_island[i] = scaleMatrix_island[i] * rotateMatrix_island[i] * transMatrix_island[i];
		m_Island->SetCollider(scaleMatrix_island[i], transMatrix_island[i]);//colliderSet

		isCollide_island[i] = m_Player->checkCollider(m_Island->GetMaxX(), m_Island->GetMinX(), m_Island->GetMaxY(),
			m_Island->GetMinY(), m_Island->GetMaxZ(), m_Island->GetMinZ());

		if (isCollide_island[i])
		{
			if (currentSpeedState > 0) {
				collisionFront = true;
			}
			else if (currentSpeedState < 0) {
				collisionBack = true;
			}
		}
	}

	for (int i = 0; i < 5; i++) {
		worldMatrix_harbor[i] = scaleMatrix_harbor * rotateMatrix_harbor[i] * transMatrix_harbor[i];
		m_Harbor->SetCollider(scaleMatrix_harbor, transMatrix_harbor[i]);//colliderSet

		isCollide_harbor[i] = m_Player->checkCollider(m_Harbor->GetMaxX(), m_Harbor->GetMinX(), m_Harbor->GetMaxY(),
			m_Harbor->GetMinY(), m_Harbor->GetMaxZ(), m_Harbor->GetMinZ());

		if (isCollide_harbor[i])
		{
			if (currentSpeedState > 0) {
				collisionFront = true;
			}
			else if (currentSpeedState < 0) {
				collisionBack = true;
			}
		}
	}

	for (int i = 0; i < 5; i++) {
		worldMatrix_harbor[i] = scaleMatrix_harbor * rotateMatrix_harbor[i] * transMatrix_harbor[i];
		m_Harbor->SetCollider(scaleMatrix_harbor, transMatrix_harbor[i]);//colliderSet

		isCollide_harbor[i] = m_Player->checkCollider(m_Harbor->GetMaxX(), m_Harbor->GetMinX(), m_Harbor->GetMaxY(),
			m_Harbor->GetMinY(), m_Harbor->GetMaxZ(), m_Harbor->GetMinZ());

		if (isCollide_harbor[i])
		{
			if (currentSpeedState > 0) {
				collisionFront = true;
			}
			else if (currentSpeedState < 0) {
				collisionBack = true;
			}
		}
	}

	for (int i = 0; i < 3; i++) {
		if ((m_Convoy[i]->checkCollider(m_Bullet[0]->GetMaxX(), m_Bullet[0]->GetMinX(), m_Bullet[0]->GetMaxY(),
			m_Bullet[0]->GetMinY(), m_Bullet[0]->GetMaxZ(), m_Bullet[0]->GetMinZ()))
			|| (m_Convoy[i]->checkCollider(m_Bullet[1]->GetMaxX(), m_Bullet[1]->GetMinX(), m_Bullet[1]->GetMaxY(),
				m_Bullet[1]->GetMinY(), m_Bullet[1]->GetMaxZ(), m_Bullet[1]->GetMinZ()))
			|| (m_Convoy[i]->checkCollider(m_Bullet[0]->GetMaxX(), m_Bullet[2]->GetMinX(), m_Bullet[2]->GetMaxY(),
				m_Bullet[2]->GetMinY(), m_Bullet[2]->GetMaxZ(), m_Bullet[2]->GetMinZ())))
		{
			convoyHit[i] = true;
		}

		if ((m_CoastGuardCannon[i]->checkCollider(m_Bullet[0]->GetMaxX(), m_Bullet[0]->GetMinX(), m_Bullet[0]->GetMaxY(),
			m_Bullet[0]->GetMinY(), m_Bullet[0]->GetMaxZ(), m_Bullet[0]->GetMinZ()))
			|| (m_CoastGuardCannon[i]->checkCollider(m_Bullet[1]->GetMaxX(), m_Bullet[1]->GetMinX(), m_Bullet[1]->GetMaxY(),
				m_Bullet[1]->GetMinY(), m_Bullet[1]->GetMaxZ(), m_Bullet[1]->GetMinZ()))
			|| (m_CoastGuardCannon[i]->checkCollider(m_Bullet[0]->GetMaxX(), m_Bullet[2]->GetMinX(), m_Bullet[2]->GetMaxY(),
				m_Bullet[2]->GetMinY(), m_Bullet[2]->GetMaxZ(), m_Bullet[2]->GetMinZ())))
		{
			coastGuardCannonHit[i] = true;
		}
	}

	collisionWithFlag = m_Player->checkCollider(m_Flag->GetMaxX(), m_Flag->GetMinX(), m_Flag->GetMaxY(),
		m_Flag->GetMinY(), m_Flag->GetMaxZ(), m_Flag->GetMinZ());

	collisionWithContainer = m_Player->checkCollider(m_Container->GetMaxX(), m_Container->GetMinX(), m_Container->GetMaxY(),
		m_Container->GetMinY(), m_Container->GetMaxZ(), m_Container->GetMinZ());

	/// <summary>
	/// ///////////
	/// </summary>
	/// <returns></returns>

	m_Seafloor->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Seafloor->GetIndexCount(), worldMatrix_seafloor, viewMatrix, projectionMatrix,
		m_Seafloor->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), rotateMatrix_seafloor);

	//지형지물 렌더링//
	m_Island->Render(m_D3D->GetDeviceContext());

	for (int i = 0; i < 9; i++) {
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Island->GetIndexCount(), worldMatrix_island[i], viewMatrix, projectionMatrix,
			m_Island->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), 0.0f, rotateMatrix_island[i]);
	}

	m_Harbor->Render(m_D3D->GetDeviceContext());

	for (int i = 0; i < 5; i++) {
		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Harbor->GetIndexCount(), worldMatrix_harbor[i], viewMatrix, projectionMatrix,
			m_Harbor->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), rotateMatrix_harbor[i]);
	}

	m_lHouse->Render(m_D3D->GetDeviceContext());

	for (int i = 0; i < 3; i++) {
		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_lHouse->GetIndexCount(), worldMatrix_lHouse[i], viewMatrix, projectionMatrix,
			m_lHouse->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), rotateMatrix_lHouse[i]);
	}

	for (int i = 0; i < 3; i++) {
		m_Convoy[i]->Render(m_D3D->GetDeviceContext());
		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Convoy[i]->GetIndexCount(), worldMatrix_convoy[i], viewMatrix, projectionMatrix,
			m_Convoy[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), rotateMatrix_convoy[i]);

		if (!coastGuardCannonHit[i])
		{
			m_CoastGuardCannon[i]->Render(m_D3D->GetDeviceContext());
			// Render the model using the light shader.
			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_CoastGuardCannon[i]->GetIndexCount(), worldMatrix_coastGuardCannon[i], viewMatrix, projectionMatrix,
				m_CoastGuardCannon[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), rotateMatrix_coastGuardCannon[i]);
		}
	}

	m_CoastGuardBase->Render(m_D3D->GetDeviceContext());

	for (int i = 0; i < 3; i++) {
		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_CoastGuardBase->GetIndexCount(), worldMatrix_coastGuardBase[i], viewMatrix, projectionMatrix,
			m_CoastGuardBase->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), rotateMatrix_coastGuardBase[i]);
	}

	m_Rocks->Render(m_D3D->GetDeviceContext());

	for (int i = 0; i < 5; i++) {
		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Rocks->GetIndexCount(), worldMatrix_Rocks[i], viewMatrix, projectionMatrix,
			m_Rocks->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), rotateMatrix_Rocks[i]);
	}

	m_Sea->Render(m_D3D->GetDeviceContext());

	result = m_WaterShader->Render(m_D3D->GetDeviceContext(), m_Sea->GetIndexCount(), worldMatrix_sea, viewMatrix,
		projectionMatrix, reflectionMatrix, m_ReflectionTexture->GetShaderResourceView(),
		m_RefractionTexture->GetShaderResourceView(), m_Sea->GetTexture(),
		m_waterTranslation, 0.05f);
	if (!result)
	{
		return false;
	}

	m_Player->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Player->GetIndexCount(), worldMatrix_Player, viewMatrix, projectionMatrix,
		m_Player->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), rotateMatrix_Player);

	m_Turret->Render(m_D3D->GetDeviceContext());

	for (int i = 0; i < 3; i++) {
		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Turret->GetIndexCount(), worldMatrix_Turret[i], viewMatrix, projectionMatrix,
			m_Turret->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), rotateMatrix_Turret[i]);
	}
	
	for (int i = 0; i < 3; i++) {
		m_Bullet[i]->Render(m_D3D->GetDeviceContext());

		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bullet[i]->GetIndexCount(), worldMatrix_Bullet[i], viewMatrix, projectionMatrix,
			m_Bullet[i]->GetTexture());
	}

	if (!collisionWithFlag) {
		m_Flag->Render(m_D3D->GetDeviceContext());

		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Flag->GetIndexCount(), worldMatrix_flag, viewMatrix, projectionMatrix,
			m_Flag->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), rotateMatrix_flagContainer);
	}

	if (!(spacePressTime > 3.0f)) {
		m_Container->Render(m_D3D->GetDeviceContext());
		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Container->GetIndexCount(), worldMatrix_container, viewMatrix, projectionMatrix,
			m_Container->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), rotateMatrix_flagContainer);
	}

	m_D3D->TurnOffCulling();
	m_D3D->TurnOnAlphaBlending();

	static float fireTime = 0.0f;


	// 프레임 시간 카운터를 증가시킵니다.
	fireTime += 0.01f;
	if (fireTime > 1000.0f)
	{
		fireTime = 0.0f;
	}

	// 세 가지 다른 노이즈 텍스처에 대해 세 가지 스크롤 속도를 설정합니다.
	XMFLOAT3 scrollSpeeds = XMFLOAT3(1.3f, 2.1f, 2.3f);

	// 세 개의 서로 다른 노이즈 옥타브 텍스처를 만드는 데 사용할 세 개의 스케일을 설정합니다.
	XMFLOAT3 scales = XMFLOAT3(2.0f, 3.0f, 4.0f);

	// 세 가지 다른 노이즈 텍스처에 대해 세 가지 다른 x 및 y 왜곡 인수를 설정합니다.
	XMFLOAT2 distortion1 = XMFLOAT2(0.1f, 0.2f);
	XMFLOAT2 distortion2 = XMFLOAT2(0.1f, 0.3f);
	XMFLOAT2 distortion3 = XMFLOAT2(0.1f, 0.1f);

	// 텍스처 좌표 샘플링 섭동의 스케일과 바이어스.
	float distortionScale = 2.5f;
	float distortionBias = 1.5f;

	// 정사각형 모델의 정점과 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
	for (int i = 0; i < 3; i++) {
		m_Fire[i]->Render(m_D3D->GetDeviceContext());
		if (coastGuardCannonHit[i]) {
			// 화재 쉐이더를 사용하여 사각형 모델을 렌더링합니다.
			if (!m_FireShader->Render(m_D3D->GetDeviceContext(), m_Fire[i]->GetIndexCount(), worldMatrix_fire[i], viewMatrix, projectionMatrix,
				m_Fire[i]->GetTexture1(), m_Fire[i]->GetTexture2(), m_Fire[i]->GetTexture3(), fireTime, scrollSpeeds,
				scales, distortion1, distortion2, distortion3, distortionScale, distortionBias))
			{
				return false;
			}
		}

		m_convoyFire[i]->Render(m_D3D->GetDeviceContext());
		if (convoyHit[i]) {
			// 화재 쉐이더를 사용하여 사각형 모델을 렌더링합니다.
			if (!m_FireShader->Render(m_D3D->GetDeviceContext(), m_convoyFire[i]->GetIndexCount(), worldMatrix_convoyfire[i], viewMatrix, projectionMatrix,
				m_convoyFire[i]->GetTexture1(), m_convoyFire[i]->GetTexture2(), m_convoyFire[i]->GetTexture3(), fireTime, scrollSpeeds,
				scales, distortion1, distortion2, distortion3, distortionScale, distortionBias))
			{
				return false;
			}
		}
	}

	if (!collisionWithFlag) {
		m_Smoke_2->Render(m_D3D->GetDeviceContext());
		if (!m_SmokeShaderClass->Render(m_D3D->GetDeviceContext(), m_Smoke_2->GetIndexCount(), worldMatrix_Smoke_2, viewMatrix, projectionMatrix,
			m_Smoke_2->GetTexture()))
		{
			return false;
		}
	}
	if (!(spacePressTime > 3.0f)) {
		m_Smoke_3->Render(m_D3D->GetDeviceContext());
		if (!m_SmokeShaderClass->Render(m_D3D->GetDeviceContext(), m_Smoke_3->GetIndexCount(), worldMatrix_Smoke_3, viewMatrix, projectionMatrix,
			m_Smoke_3->GetTexture()))
		{
			return false;
		}
	}

	for (int i = 0; i < 3; i++) {
		m_baseSmoke[i]->Render(m_D3D->GetDeviceContext());
		if (coastGuardCannonHit[i]) {
			if (!m_SmokeShaderClass->Render(m_D3D->GetDeviceContext(), m_baseSmoke[i]->GetIndexCount(), worldMatrix_baseSmoke[i], viewMatrix, projectionMatrix,
				m_baseSmoke[i]->GetTexture()))
			{
				return false;
			}
		}

		m_convoySmoke[i]->Render(m_D3D->GetDeviceContext());
		if (convoyHit[i]) {
			if (!m_SmokeShaderClass->Render(m_D3D->GetDeviceContext(), m_convoySmoke[i]->GetIndexCount(), worldMatrix_convoySmoke[i], viewMatrix, projectionMatrix,
				m_convoySmoke[i]->GetTexture()))
			{
				return false;
			}
		}
	}

	m_D3D->TurnZBufferOff();

	m_Smoke_1->Render(m_D3D->GetDeviceContext());
	if (!m_SmokeShaderClass->Render(m_D3D->GetDeviceContext(), m_Smoke_1->GetIndexCount(), worldMatrix_Smoke, viewMatrix, projectionMatrix,
		m_Smoke_1->GetTexture()))
	{
		return false;
	}
	m_D3D->TurnOnCulling();

	////////////////////////////////////////////////////////////////////////////////
// UI 배치 시작하는 부분.
////////////////////////////////////////////////////////////////////////////////

	if (isMenuPressed || isRetryMenuPressed) {
		if (isMenuPressed) {
			m_inGameMenu->Render(m_D3D->GetDeviceContext(), (screenwidth / 2) - 200, (screenheight / 2) - 300);

			m_TextureShader->Render(m_D3D->GetDeviceContext(), m_inGameMenu->GetIndexCount(), worldMatrix_UIBG,
				viewMatrix, orthoMatrix, m_inGameMenu->GetTexture());
		}
		if (isRetryMenuPressed) {
			m_RetrytMenu->Render(m_D3D->GetDeviceContext(), (screenwidth / 2) - 200, (screenheight / 2) - 300);

			m_TextureShader->Render(m_D3D->GetDeviceContext(), m_RetrytMenu->GetIndexCount(), worldMatrix_UIBG,
				viewMatrix, orthoMatrix, m_RetrytMenu->GetTexture());
		}
	}

	else {
		if (clear == true) {
			m_Clear->Render(m_D3D->GetDeviceContext(), (screenwidth / 2) - 200, (screenheight / 2) - 300);

			m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Clear->GetIndexCount(), worldMatrix_UIBG,
				viewMatrix, orthoMatrix, m_Clear->GetTexture());
		}
		if (fail == true) {
			m_Fail->Render(m_D3D->GetDeviceContext(), (screenwidth / 2) - 200, (screenheight / 2) - 300);

			m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Fail->GetIndexCount(), worldMatrix_UIBG,
				viewMatrix, orthoMatrix, m_Fail->GetTexture());
		}

		XMFLOAT4 clipPlane = XMFLOAT4(0.0f, 0.1f, 0.0f, 0.0f);

		if (collisionWithContainer) {
			m_PressSpace->Render(m_D3D->GetDeviceContext(), (screenwidth / 2) - 150, (screenheight / 2) - 300);

			m_TextureShader->Render(m_D3D->GetDeviceContext(), m_PressSpace->GetIndexCount(), worldMatrix_UIBG,
				viewMatrix, orthoMatrix, m_PressSpace->GetTexture());

			m_HoldBar->Render(m_D3D->GetDeviceContext(), (screenwidth / 2) + (screenwidth / 4), (screenheight / 2) - 40 - 500 * (spacePressTime / 5.0f));

			m_ClipPlaneShaderClass->Render(m_D3D->GetDeviceContext(), m_HoldBar->GetIndexCount(), worldMatrix_Gage,
				viewMatrix, orthoMatrix, m_HoldBar->GetTexture(), clipPlane);

			m_HoldBarFrame->Render(m_D3D->GetDeviceContext(), (screenwidth / 2) + (screenwidth / 4), (screenheight / 2) - 335);

			m_TextureShader->Render(m_D3D->GetDeviceContext(), m_HoldBarFrame->GetIndexCount(), worldMatrix_UIBG,
				viewMatrix, orthoMatrix, m_HoldBarFrame->GetTexture());
		}

		if (collisionFront || isBoundary || collisionBack) {
			m_CollisionWarning->Render(m_D3D->GetDeviceContext(), (screenwidth / 2) - 100, (screenheight / 2) - 300);

			m_TextureShader->Render(m_D3D->GetDeviceContext(), m_CollisionWarning->GetIndexCount(), worldMatrix_UIBG,
				viewMatrix, orthoMatrix, m_CollisionWarning->GetTexture());
		}
	}

	if (isAimed) {
		result = m_Crosshair_Aimed_Arm->Render(m_D3D->GetDeviceContext(),
			(screenwidth / 2) - ((screenwidth * 0.1) / 2) - aimedArmDis, (screenheight / 2) - ((screenwidth * 0.002) / 2));

		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Crosshair_Aimed_Arm->GetIndexCount(), worldMatrix_CHA_arm[0],
			viewMatrix, orthoMatrix, m_Crosshair_Aimed_Arm->GetTexture());
		if (!result)
		{
			return false;
		}

		result = m_Crosshair_Aimed_Arm->Render(m_D3D->GetDeviceContext(),
			(screenwidth / 2) - ((screenwidth * 0.1) / 2) + aimedArmDis, (screenheight / 2) - ((screenwidth * 0.002) / 2));

		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Crosshair_Aimed_Arm->GetIndexCount(), worldMatrix_CHA_arm[1],
			viewMatrix, orthoMatrix, m_Crosshair_Aimed_Arm->GetTexture());
		if (!result)
		{
			return false;
		}

		result = m_Crosshair_Aimed->Render(m_D3D->GetDeviceContext(), 
			(screenwidth / 2) - ((screenwidth * 0.006)/2), (screenheight / 2) - ((screenwidth * 0.006) / 2));
		if (!result)
		{
			return false;
		}

		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Crosshair_Aimed->GetIndexCount(), worldMatrix_CHA,
			viewMatrix, orthoMatrix, m_Crosshair_Aimed->GetTexture());
		if (!result)
		{
			return false;
		}
	}

	else {
		// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.

		result = m_Crosshair_Arm->Render(m_D3D->GetDeviceContext(),
			(screenwidth / 2) - ((screenwidth * 0.1) / 2) - ArmDis, (screenheight / 2) - ((screenwidth * 0.002) / 2));

		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Crosshair_Arm->GetIndexCount(), worldMatrix_CH_arm[0],
			viewMatrix, orthoMatrix, m_Crosshair_Arm->GetTexture());
		if (!result)
		{
			return false;
		}

		result = m_Crosshair_Arm->Render(m_D3D->GetDeviceContext(),
			(screenwidth / 2) - ((screenwidth * 0.1) / 2) + ArmDis, (screenheight / 2) - ((screenwidth * 0.002) / 2));

		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Crosshair_Arm->GetIndexCount(), worldMatrix_CH_arm[1],
			viewMatrix, orthoMatrix, m_Crosshair_Arm->GetTexture());
		if (!result)
		{
			return false;
		}

		// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		result = m_Crosshair->Render(m_D3D->GetDeviceContext(), 
			(screenwidth / 2) - ((screenwidth * 0.006) / 2), (screenheight / 2) - ((screenwidth * 0.006) / 2));
		if (!result)
		{
			return false;
		}

		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Crosshair->GetIndexCount(), worldMatrix_CH,
			viewMatrix, orthoMatrix, m_Crosshair->GetTexture());
		if (!result)
		{
			return false;
		}
	}

	m_UIBG->Render(m_D3D->GetDeviceContext(), 10.0f, 10.0f);

	m_TextureBlendShader->Render(m_D3D->GetDeviceContext(), m_UIBG->GetIndexCount(), worldMatrix_UIBG,
		viewMatrix, orthoMatrix, m_UIBG->GetTexture());

	m_UIBG->Render(m_D3D->GetDeviceContext(), screenwidth - 220.0f, 10.0f);

	m_TextureBlendShader->Render(m_D3D->GetDeviceContext(), m_UIBG->GetIndexCount(), worldMatrix_UIBG,
		viewMatrix, orthoMatrix, m_UIBG->GetTexture());

	m_UIBG2->Render(m_D3D->GetDeviceContext(), 10.0f, screenheight - 220.0f);

	m_TextureBlendShader->Render(m_D3D->GetDeviceContext(), m_UIBG2->GetIndexCount(), worldMatrix_UIBG,
		viewMatrix, orthoMatrix, m_UIBG2->GetTexture());

	m_shipUI->Render(m_D3D->GetDeviceContext(), 0.0f, 0.0f);

	m_TextureShader->Render(m_D3D->GetDeviceContext(), m_shipUI->GetIndexCount(), worldMatrix_shipUI,
		viewMatrix, orthoMatrix, m_shipUI->GetTexture());

	m_turnArrowUI->Render(m_D3D->GetDeviceContext(), 0.0f, 0.0f);

	m_TextureShader->Render(m_D3D->GetDeviceContext(), m_turnArrowUI->GetIndexCount(), worldMatrix_TurnBar,
		viewMatrix, orthoMatrix, m_turnArrowUI->GetTexture());

	m_turnUI->Render(m_D3D->GetDeviceContext(), (screenwidth / 2) - 250, screenheight - 265);

	m_TextureShader->Render(m_D3D->GetDeviceContext(), m_turnUI->GetIndexCount(), worldMatrix_UIBG,
		viewMatrix, orthoMatrix, m_turnUI->GetTexture());

	m_SNUI->Render(m_D3D->GetDeviceContext(), 18.0f, screenheight - 210.0f);

	m_TextureShader->Render(m_D3D->GetDeviceContext(), m_SNUI->GetIndexCount(), worldMatrix_UIBG,
		viewMatrix, orthoMatrix, m_SNUI->GetTexture());

	// 텍스트 출력부분
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix_Text, orthoMatrix);
	if (!result)
	{
		return false;
	}

	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();

	m_D3D->EndScene();

	return true;
}

XMMATRIX GraphicsClass::MoveChar(float playerYaw, float playerPitch, float playerRoll, float moveLeftRight, 
								 float moveBackForward, XMVECTOR DefaultForward, XMVECTOR DefalutRight)

{
	XMMATRIX rotateMatrix_Player;
	XMMATRIX rotateMatrix_Player_Tilt;
	XMMATRIX DefaultTilt;

	rotateMatrix_Player = XMMatrixRotationRollPitchYaw(0, playerYaw, playerRoll);

	playerTarget = XMVector3TransformNormal(DefaultForward, rotateMatrix_Player);

	playerRight = XMVector3TransformCoord(DefalutRight, rotateMatrix_Player);
	playerForward = XMVector3TransformCoord(DefaultForward, rotateMatrix_Player);
	playerUp = XMVector3Cross(playerForward, playerRight);

	playerPosition += moveLeftRight * playerRight;
	playerPosition += moveBackForward * playerForward;

	playerTarget = playerPosition + playerTarget;

	transMatrix_Player = XMMatrixTranslation(XMVectorGetX(playerPosition), XMVectorGetY(playerPosition), XMVectorGetZ(playerPosition));

	rotateMatrix_Player_Tilt = XMMatrixRotationRollPitchYaw(playerPitch, 0, 0);
	DefaultTilt = XMMatrixRotationRollPitchYaw(0, 0, 0);

	worldMatrix_Player = scaleMatrix_Player * DefaultTilt * rotateMatrix_Player_Tilt * rotateMatrix_Player * transMatrix_Player;

	return rotateMatrix_Player;
}


float GraphicsClass::LookAt(XMVECTOR ObjPos, XMVECTOR DefaultForward, XMVECTOR DefalutRight, float playerYaw)
{
	XMMATRIX rotateMatrix_Player;
	rotateMatrix_Player = XMMatrixRotationRollPitchYaw(0, playerYaw, 0);

	Cross_Dir = ObjPos;

	crossForward = XMVector3TransformCoord(DefaultForward, rotateMatrix_Player);
	crossRight = XMVector3TransformCoord(DefalutRight, rotateMatrix_Player);
	Cross_Dir = XMVector3Normalize(Cross_Dir);

	Dot = XMVector3Dot(crossForward, Cross_Dir);
	Radian = XMVectorACos(Dot);

	Cross_Up = XMVector3Cross(crossForward, crossRight);

	XMVECTOR dotExam = XMVector3Dot(crossRight, Cross_Dir);

	Rad = XMVectorGetX(Radian);
	rotateCCS = XMVectorGetX(dotExam);

	if (rotateCCS > 0) {
		if (turretRad < -XMConvertToRadians(90)) turretRad = turretRad + XMConvertToRadians(360);
		return Rad;
	}
	else {
		if (turretRad > XMConvertToRadians(90)) turretRad = turretRad - XMConvertToRadians(360);
		return -Rad;
	}
}