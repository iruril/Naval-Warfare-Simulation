////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <directxmath.h>

#include "AlignedAllocationPolicy.h"

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass : public AlignedAllocationPolicy<16>
{
private:
	struct ColliderType
	{
		float Mx, mx;
		float My, my;
		float Mz, mz;
	};
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render(float, float, float, float, XMVECTOR, XMVECTOR, XMVECTOR, float, float);
	void GetViewMatrix(XMMATRIX&);

	void RenderReflection(float, float, float, float, XMVECTOR, XMVECTOR, XMVECTOR, float, float, float);
	void GetReflectionViewMatrix(XMMATRIX&);

	void RenderFreeLookCam(float, float, float, float, XMVECTOR, XMVECTOR);
	void RenderFreeLookCamReflection(float, float, float, float, XMVECTOR, XMVECTOR, float, float);

	XMVECTOR GetLookDir();

	XMVECTOR camPosition;
	XMVECTOR r_camPosition;

	XMVECTOR camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR r_camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR r_camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR r_camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR r_camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);


	XMFLOAT3 m_camPosition;
	XMFLOAT3 m_r_camPosition;

	XMFLOAT3 m_camTarget;
	XMFLOAT3 m_r_camTarget;

	XMFLOAT3 m_camForward;
	XMFLOAT3 m_r_camForward;
	
	void setCollider();
	void setSize(float, float, float);
	bool checkCollider(float, float, float, float, float, float);
	void Shutdown();

private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;

	XMFLOAT3 m_r_position;
	XMFLOAT3 m_r_rotation;

	XMMATRIX m_viewMatrix;
	XMMATRIX m_reflectionViewMatrix;

	ColliderType* m_collider;
	float sizX, sizY, sizZ;
};

#endif