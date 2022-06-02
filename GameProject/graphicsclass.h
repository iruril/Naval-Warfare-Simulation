////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#define FRONT 1
#define STOP 0
#define BACK -1

#define TYPE_1 0
#define TYPE_2 1
#define TYPE_3 2
#define TYPE_4 3

#define FAR_AWAY 5000.0f

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "skymapclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"

#include "textureshaderclass.h"
#include "textureblendshaderclass.h"
#include "bitmapclass.h"
#include "textclass.h"

#include "rendertextureclass.h"
#include "refractionshaderclass.h"
#include "watershaderclass.h"
#include "ParticleSystemClass.h"
#include "ParticleShaderClass.h"

#include "ClipPlaneShaderClass.h"
#include "ProjectileClass.h"
#include "fireshaderclass.h"
#include "firemodelclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1500.0f;
const float SCREEN_NEAR = 1.0f;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, float, int, int, float, int);

	bool isFreeLookMode = false;

	bool shooting = false;
	bool isAimed = false;
	int reloadTime = 0;

	bool isMaintitle = true;
	bool isGameScene = false;
	bool isResultScene = false;

	bool isMenuPressed = false;

	bool isRetryMenuPressed = false;
	bool isRetryPressed = false;

	float m_fps_Calculate = 1.0f;
	float turnSpeed = 0.0f;

	float charCamDist = 60.0f;
	float charCamHeight = 12.0f;
	float CamHeight = 0.0f;

	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;

	float CAMmoveLeftRight = 0.0f;
	float CAMmoveBackForward = 0.0f;

	int currentSpeedState = 0;

	float camRotateInfoYaw = 0.0f;
	float camRotateInfoPitch = 0.0f;

	float playerYaw = 0.0f;
	float playerPitch = 0.0f;
	float playerRoll = 0.0f;

	float camYaw = 0.0f;
	float camPitch = 0.0f;

	bool collisionFront = false;
	bool collisionBack = false;

	bool collisionWithFlag = false;
	bool collisionWithContainer = false;

	float spacePressTime = 0.0f;
	float m_time = 0.0f;

	bool isCollide_island[9];
	bool isCollide_harbor[5];
	bool isBoundary;

	int num_island = 0;
	int num_harbor = 0;

	bool clear = false;
	bool fail = false;

private:
	bool Render();
	bool RenderTutorialScene();
	bool RenderMainTitleScene(); // 메인 타이틀
	bool RenderResultScene(); //실패 시 메인메뉴로 돌아가거나, 성공 시 메인메뉴로 돌아가기 전 출력.

	bool RenderTutorialGameRefractionToTexture();
	bool RenderTutorialGameReflectionToTexture();
	bool RenderTutorialGameScene();

	void SetPosition();
	void RestartScene();

	XMMATRIX MoveChar(float, float, float, float, float, XMVECTOR, XMVECTOR);
	float LookAt(XMVECTOR, XMVECTOR, XMVECTOR, float);
	float LookAt(XMFLOAT3, XMVECTOR, XMVECTOR, XMVECTOR);

	typedef struct EnemyCoastGuard {
		XMFLOAT3 position;
		int health = 3;
		bool isDestroyed = false;
		bool isHit = false;
		XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	}EnemyCoastGuard;

	typedef struct EnemyShip {
		XMFLOAT3 position;
		int health = 3;
		bool isDestroyed = false;
		bool isHit = false;
	}EnemyShip;

private:
	int flagNum = 4;
	int containerNum = 3;
	int EnemyShipNum = 3;
	int EnemyCoastGuardGun = 3;

	int wayPointNum = 8;
	float transFlagX[8] = { FAR_AWAY,  65.0f, FAR_AWAY, -70.0f,  -200.0f, FAR_AWAY, -220.0f, FAR_AWAY };
	float transFlagZ[8] = { FAR_AWAY,  53.0f, FAR_AWAY, 96.0f,  440.0f, FAR_AWAY, -170.0f, FAR_AWAY };

	float transContainX[8] = { 0.0f,  FAR_AWAY,  -9.0f,  FAR_AWAY,  FAR_AWAY, -430.0f, FAR_AWAY, FAR_AWAY };
	float transContainZ[8] = { 550.0f,  FAR_AWAY,  -64.0f,  FAR_AWAY,  FAR_AWAY, 130.0f, FAR_AWAY, FAR_AWAY };

	float rotate_Entity = 0.0f;
	//내 기록-> 2분 11초. (화물 잡는 시간 5초 포함하면 약 2분 30초 남짓 예상함.) 즉, 최대 3분30초 부여.

	int pitch = 0;
	int speed = 0;

	float frameTime = 0.0f;

	int screenwidth;
	int screenheight;

	int m_mouseX = 0;
	int m_mouseY = 0;

	int num_total = 0;

	float transTurretY[3] = { -0.1f,  -0.575f, -0.65f };
	float transTurretZ[3] = { -3.85f,  -7.2f, 10.2f };

	D3DClass* m_D3D;
	CameraClass* m_Camera;

	SkymapClass* m_SkyMap;
	ModelClass* m_Sea;
	ModelClass* m_Seafloor;
	ModelClass* m_Player;
	ModelClass* m_Turret;
	ModelClass* m_Island;
	ModelClass* m_Harbor;
	ModelClass* m_lHouse;
	ModelClass* m_Rocks;
	ModelClass* m_Flag;
	ModelClass* m_Container;
	ModelClass* m_Convoy[3];
	ModelClass* m_CoastGuardBase;
	ModelClass* m_CoastGuardCannon[3];
	FireModelClass* m_Fire[3];
	FireModelClass* m_convoyFire[3];

	ProjectileClass* m_Bullet[3];

	ParticleSystemClass* m_baseSmoke[3];
	ParticleSystemClass* m_convoySmoke[3];

	ParticleSystemClass* m_Smoke_1;
	ParticleSystemClass* m_Smoke_2;
	ParticleSystemClass* m_Smoke_3;

	BitmapClass* m_Crosshair;
	BitmapClass* m_Crosshair_Aimed;
	BitmapClass* m_Crosshair_Arm;
	BitmapClass* m_Crosshair_Aimed_Arm;

	BitmapClass* m_UIBG;
	BitmapClass* m_UIBG2;
	BitmapClass* m_shipUI;
	BitmapClass* m_SNUI;
	BitmapClass* m_turnUI;
	BitmapClass* m_turnArrowUI;
	BitmapClass* m_PressSpace;
	BitmapClass* m_HoldBar;
	BitmapClass* m_HoldBarFrame;
	BitmapClass* m_CollisionWarning;
	BitmapClass* m_Clear;
	BitmapClass* m_Fail;
	BitmapClass* m_Menu;
	BitmapClass* m_UIBGforTitle;
	BitmapClass* m_SuccessResultScene;
	BitmapClass* m_FailResultScene;
	BitmapClass* m_MainTitle;
	BitmapClass* m_RetrytMenu;
	BitmapClass* m_inGameMenu;

	TextClass* m_Text;

	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	TextureShaderClass* m_TextureShader;
	TextureBlendShaderClass* m_TextureBlendShader;
	RenderTextureClass* m_RefractionTexture, * m_ReflectionTexture;
	RefractionShaderClass* m_RefractionShader;
	WaterShaderClass* m_WaterShader;
	float m_waterHeight, m_waterTranslation;
	XMFLOAT4 WaterDiffuseColor = XMFLOAT4(0.2f, 0.4f, 0.8f, 1.0f);
	XMFLOAT4 WaterAmbientColor = XMFLOAT4(0.3f, 0.7f, 1.0f, 1.0f);

	ParticleShaderClass* m_SmokeShaderClass;
	ClipPlaneShaderClass* m_ClipPlaneShaderClass;
	FireShaderClass* m_FireShader = nullptr;

	/// <summary>
	/// /////////////////////////////////////////////////////////
	/// </summary>

	XMVECTOR desiredCharDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR playerPosition = XMVectorSet(0.0f, 0.2f, 700.0f, 0.0f);

	XMVECTOR playerPosition4Cam = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR crossForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	XMVECTOR crossRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR Cross_Dir;
	XMVECTOR Cross_Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMVECTOR Dot = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Radian;

	XMVECTOR CamLookDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	XMFLOAT3 m_playerPosition;

	XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR CamDefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR CamDefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR playerTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR playerUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMVECTOR playerForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR playerRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	bool isAssend = false;
	bool isTilt = false;

	float accelate_Assend = 0.000f; //add 0.00001f
	float accelate_Tilt = 0.0f; //add 0.0001f * 0.0174532925f;

	float accelate_Assend_Limit = 0.0015f;
	float accelate_Tilt_Limit = 0.00005f;


	float rotateCCS;
	float Rad;
	float rotateRad;
	float rotateRadPrev;
	float turretRad = 0.0f;
	float m_turretRad;
	float turretTurnRate = 0.15 * 0.0174532925f;

	XMVECTOR Vector_Turret[3];
	XMVECTOR Vector_Turret_Forward[3];
	XMVECTOR Vector_Turret_Right[3];
	XMVECTOR Vector_Turret_Up[3];

	/*D3DXVECTOR3 currCharDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 oldCharDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 charPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);*/

	XMMATRIX worldMatrix_Player;
	XMMATRIX transMatrix_Player;
	XMMATRIX rotateMatrix_Player;
	XMMATRIX scaleMatrix_Player = XMMatrixScaling(2.2f, 2.2f, 2.2f);
	XMMATRIX scaleMatrix_Player_for_Collider = XMMatrixScaling(1.2f, 1.2f, 1.2f);

	XMMATRIX worldMatrix_Turret[3];
	XMMATRIX scaleMatrix_Turret = XMMatrixScaling(2.2f, 2.2f, 2.2f);
	XMMATRIX transMatrix_Turret[3];
	XMMATRIX rotateMatrix_Turret[3];
	int num_turret = 0;

	///////////////////////////////////////////////////////////////

	XMMATRIX worldMatrix_skymap;
	XMMATRIX transMatrix_skymap;
	XMMATRIX scaleMatrix_skymap;
	XMMATRIX rotateMatrix_skymap;

	XMMATRIX worldMatrix_sea;
	XMMATRIX scaleMatrix_sea;
	XMMATRIX transMatrix_sea;

	XMMATRIX worldMatrix_seafloor;
	XMMATRIX scaleMatrix_seafloor;
	XMMATRIX transMatrix_seafloor;
	XMMATRIX rotateMatrix_seafloor;

	XMMATRIX worldMatrix_island[9];
	XMMATRIX scaleMatrix_island[9];
	XMMATRIX transMatrix_island[9];
	XMMATRIX rotateMatrix_island[9];

	XMMATRIX worldMatrix_Rocks[5];
	XMMATRIX scaleMatrix_Rocks = XMMatrixScaling(3.2f, 3.2f, 3.2f);
	XMMATRIX transMatrix_Rocks[5];
	XMMATRIX rotateMatrix_Rocks[5];
	int num_rocks = 0;

	XMMATRIX worldMatrix_harbor[5];
	XMMATRIX scaleMatrix_harbor = XMMatrixScaling(2.2f, 2.2f, 2.2f);
	XMMATRIX transMatrix_harbor[5];
	XMMATRIX rotateMatrix_harbor[5];

	XMMATRIX worldMatrix_lHouse[3];
	XMMATRIX scaleMatrix_lHouse = XMMatrixScaling(2.2f, 2.2f, 2.2f);
	XMMATRIX transMatrix_lHouse[3];
	XMMATRIX rotateMatrix_lHouse[3];
	int num_iHouse = 0;

	EnemyShip convoyInfo[3];

	XMMATRIX worldMatrix_convoy[3];
	XMMATRIX scaleMatrix_convoy = XMMatrixScaling(2.2f, 2.2f, 2.2f);
	XMMATRIX transMatrix_convoy[3];
	XMMATRIX rotateMatrix_convoy[3];
	int num_convoy = 0;

	XMMATRIX worldMatrix_coastGuardBase[3];
	XMMATRIX scaleMatrix_coastGuardBase = XMMatrixScaling(2.2f, 2.2f, 2.2f);
	XMMATRIX transMatrix_coastGuardBase[3];
	XMMATRIX rotateMatrix_coastGuardBase[3];
	int num_coastGuardBase = 0;

	EnemyCoastGuard coastCannonInfo[3];

	XMMATRIX worldMatrix_coastGuardCannon[3];
	XMMATRIX scaleMatrix_coastGuardCannon = XMMatrixScaling(2.2f, 2.2f, 2.2f);
	XMMATRIX transMatrix_coastGuardCannon[3];
	XMMATRIX rotateMatrix_coastGuardCannon[3];
	int num_coastGuardCannon = 0;

	float m_indexCnt = 0;

	XMMATRIX worldMatrix_fire[3];
	XMMATRIX scaleMatrix_fire = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	XMMATRIX transMatrix_fire = XMMatrixTranslation(0.0f, 5.0f, 0.0f);
	XMMATRIX rotateMatrix_fire;

	XMMATRIX worldMatrix_convoyfire[3];
	XMMATRIX scaleMatrix_convoyfire = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	XMMATRIX transMatrix_convoyfire = XMMatrixTranslation(0.0f, 5.0f, 0.0f);
	XMMATRIX rotateMatrix_convoyfire;

	XMMATRIX rotateMatrix_flagContainer;
	XMMATRIX transMatrix_flagContainer;

	XMMATRIX worldMatrix_flag;
	XMMATRIX scaleMatrix_flag;
	XMMATRIX scaleMatrix_flag_for_Collision = XMMatrixScaling(10.0f, 10.0f, 10.0f);

	XMMATRIX worldMatrix_container;
	XMMATRIX scaleMatrix_container;
	XMMATRIX scaleMatrix_container_for_Collision = XMMatrixScaling(10.0f, 10.0f, 10.0f);


	XMMATRIX worldMatrix_Bullet[3];

	///////////////////////////////////////////////////////////////

	XMMATRIX scaleMatrix_Smoke = XMMatrixScaling(2.0f, 2.0f, 2.0f);

	XMMATRIX worldMatrix_baseSmoke[3];
	XMMATRIX rotateMatrix_baseSmoke;

	XMMATRIX worldMatrix_convoySmoke[3];
	XMMATRIX rotateMatrix_convoySmoke;

	XMMATRIX worldMatrix_Smoke;
	XMMATRIX transMatrix_Smoke;
	XMMATRIX rotateMatrix_Smoke;

	XMMATRIX worldMatrix_Smoke_2;
	XMMATRIX transMatrix_Smoke_2;
	XMMATRIX rotateMatrix_Smoke_2;

	XMMATRIX worldMatrix_Smoke_3;
	XMMATRIX transMatrix_Smoke_3;
	XMMATRIX rotateMatrix_Smoke_3;

	///////////////////////////////////////////////////////////////

	XMMATRIX worldMatrix_Text;
	XMMATRIX transMatrix_Text;
	XMMATRIX rotateMatrix_Text;

	XMMATRIX worldMatrix_UIBG;
	XMMATRIX rotateMatrix_UIBG;

	XMMATRIX worldMatrix_shipUI;
	XMMATRIX transMatrix_shipUI;
	XMMATRIX transMatrix_shipUIp;
	XMMATRIX rotateMatrix_shipUI;
	XMMATRIX rotateMatrix_shipUIr;

	XMMATRIX worldMatrix_TurnBar;
	XMMATRIX transMatrix_TurnBarC;
	XMMATRIX transMatrix_TurnBar;
	XMMATRIX rotateMatrix_TurnBar;

	XMMATRIX worldMatrix_CH;
	XMMATRIX rotateMatrix_CH;

	XMMATRIX worldMatrix_CHA;
	XMMATRIX rotateMatrix_CHA;

	XMMATRIX worldMatrix_CH_arm[2];
	XMMATRIX rotateMatrix_CH_arm[2];

	XMMATRIX worldMatrix_CHA_arm[2];
	XMMATRIX rotateMatrix_CHA_arm[2];

	XMMATRIX worldMatrix_Gage;

	int aimedArmDis = 80;
	int ArmDis = 160; // 320 max
	bool aimDifferCheck = false;

};

#endif