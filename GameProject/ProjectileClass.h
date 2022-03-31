////////////////////////////////////////////////////////////////////////////////
// Filename: ProjectileClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PROJECTILECLASS_H_
#define _PROJECTILECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"

#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: ProjectileClass
////////////////////////////////////////////////////////////////////////////////
class ProjectileClass
{
public:
	bool shooted = false;
	bool inWater = false;


	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;
	float moveUpDown = 0.0f;

private:
	float projectilePitch = 0.0f;
	float projectileYaw = 0.0f;
	float speed = 0.6f;
	float gravityForce = 0.001f;

	XMVECTOR projectilePosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR projectileTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR projectileUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMVECTOR projectileForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR projectileRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct ColliderType//콜라이더형 추가
	{
		float Mx, mx;
		float My, my;
		float Mz, mz;
	};

	struct Vertex	//Overloaded Vertex Structure
	{
		Vertex() {}
		Vertex(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz)
			: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
	};

	typedef struct
	{
		float x, y, z;
	}VertexFile;

	typedef struct
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	}FaceFile;

public:
	ProjectileClass();
	ProjectileClass(const ProjectileClass&);
	~ProjectileClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	XMMATRIX ProjectileShoot(XMVECTOR, float, float);
	void ProjReload(XMVECTOR);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	float GetMaxX();//사이즈 반환 함수 추가
	float GetMinX();
	float GetMaxY();
	float GetMinY();
	float GetMaxZ();
	float GetMinZ();//-------------------

	void SetCollider(XMMATRIX, XMMATRIX);//콜라이더 설정 함수 추가
	void SetCollider();
	bool checkCollider(float, float, float, float, float, float);
	int getPolygon();

private:
	void initProjectileControl(XMVECTOR, float, float);
	XMMATRIX ProjectileControl();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

	void ObjconvertToTxt(char*, char*);
	void GetModelFilename(char*);
	bool ReadFileCounts(char*, int&, int&, int&, int&);
	bool LoadDataStructures(char*, int, int, int, int, char*);

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	ModelType* m_projectile;
	const char* txt = ".txt";

	ColliderType* m_collider;//콜라이더 변수 추가
	float Max, Mix, May, Miy, Maz, Miz;//모델 사이즈 변수 추가

	XMMATRIX worldMatrix_projectile;
	XMMATRIX transMatrix_projectile;
	XMMATRIX rotateMatrix_projectile;
	XMMATRIX scaleMatrix_projectile = XMMatrixScaling(1, 1, 1);
};

#endif
