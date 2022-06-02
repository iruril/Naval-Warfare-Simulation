////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_rotation.x = 0.0f;
	m_rotation.y = 0.0f;
	m_rotation.z = 0.0f;

	m_collider = new ColliderType;//콜라이더 타입 변수 추가
	sizX = sizY = sizZ = 0.0f;//카메라 사이즈 변수 추가
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	return;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
	return;
}


XMFLOAT3 CameraClass::GetPosition()
{
	XMStoreFloat3(&m_position, camPosition);
	return m_position;
}


XMFLOAT3 CameraClass::GetRotation()
{
	XMStoreFloat3(&m_rotation, camTarget);
	return m_rotation;
}

// This uses the position and rotation of the camera to build and to update the view matrix.
void CameraClass::Render(float camYaw, float camPitch, float moveLeftRight, float moveBackForward,
	XMVECTOR DefaultForward, XMVECTOR DefalutRight, XMVECTOR charPosition, float charCamDist, float charCamHeight)
{
	XMMATRIX camRotationMatrix;

	camTarget = charPosition;
	XMStoreFloat3(&m_camTarget, camTarget);
	m_camTarget.y = m_camTarget.y + charCamHeight;
	camTarget = XMLoadFloat3(&m_camTarget);


	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camPosition = XMVector3TransformNormal(DefaultForward, camRotationMatrix);
	camPosition = XMVector3Normalize(camPosition);

	XMStoreFloat3(&m_camPosition, camPosition);
	XMStoreFloat3(&m_camTarget, camTarget);

	m_camPosition.x = (m_camPosition.x * charCamDist) + m_camTarget.x;
	m_camPosition.y = (m_camPosition.y * charCamDist) + m_camTarget.y;
	m_camPosition.z = (m_camPosition.z * charCamDist) + m_camTarget.z;

	camPosition = XMLoadFloat3(&m_camPosition);
	camTarget = XMLoadFloat3(&m_camTarget);

	XMFLOAT3 Tar_Pos(m_camTarget.x - m_camPosition.x, m_camTarget.y - m_camPosition.y, m_camTarget.z - m_camPosition.z);
	XMVECTOR Tar_Pos_Vec = XMLoadFloat3(&Tar_Pos);

	camForward = XMVector3Normalize(Tar_Pos_Vec);

	camForward = XMVector3Normalize(camForward);
	XMStoreFloat3(&m_camForward, camForward);
	camRight = XMVectorSet(-m_camForward.z, 0.0f, m_camForward.x, 0.0f);

	XMFLOAT3 Pos_Tar(m_camPosition.x - m_camTarget.x, m_camPosition.y - m_camTarget.y, m_camPosition.z - m_camTarget.z);
	XMVECTOR Pos_Tar_Vec = XMLoadFloat3(&Pos_Tar);
	Pos_Tar_Vec = XMVector3Normalize(Pos_Tar_Vec);

	camUp = XMVector3Normalize(XMVector3Cross(Pos_Tar_Vec, camRight));

	m_viewMatrix = XMMatrixLookAtLH(camPosition, camTarget, camUp);
	return;
}

void CameraClass::RenderReflection(float camYaw, float camPitch, float moveLeftRight, float moveBackForward,
	XMVECTOR DefaultForward, XMVECTOR DefalutRight, XMVECTOR charPosition, float charCamDist, float charCamHeight, float height)
{
	XMMATRIX camRotationMatrix;

	r_camTarget = charPosition;
	XMStoreFloat3(&m_r_camTarget, r_camTarget);
	m_r_camTarget.y = -(m_r_camTarget.y + charCamHeight) + (height * 2.0f);
	r_camTarget = XMLoadFloat3(&m_r_camTarget);

	camRotationMatrix = XMMatrixRotationRollPitchYaw(-camPitch, camYaw, 0);
	r_camPosition = XMVector3TransformNormal(DefaultForward, camRotationMatrix);
	r_camPosition = XMVector3Normalize(r_camPosition);

	XMStoreFloat3(&m_r_camPosition, r_camPosition);
	XMStoreFloat3(&m_r_camTarget, r_camTarget);

	m_r_camPosition.x = (m_r_camPosition.x * charCamDist) + m_r_camTarget.x;
	m_r_camPosition.y = (m_r_camPosition.y * charCamDist) + m_r_camTarget.y;
	m_r_camPosition.z = (m_r_camPosition.z * charCamDist) + m_r_camTarget.z;

	r_camPosition = XMLoadFloat3(&m_r_camPosition);
	r_camTarget = XMLoadFloat3(&m_r_camTarget);

	XMFLOAT3 Tar_Pos(m_r_camTarget.x - m_r_camPosition.x, m_r_camTarget.y - m_r_camPosition.y, m_r_camTarget.z - m_r_camPosition.z);
	XMVECTOR Tar_Pos_Vec = XMLoadFloat3(&Tar_Pos);

	r_camForward = XMVector3Normalize(Tar_Pos_Vec);

	r_camForward = XMVector3Normalize(r_camForward);
	XMStoreFloat3(&m_r_camForward, r_camForward);
	r_camRight = XMVectorSet(-m_r_camForward.z, 0.0f, m_r_camForward.x, 0.0f);

	XMFLOAT3 Pos_Tar(m_r_camPosition.x - m_r_camTarget.x, m_r_camPosition.y - m_r_camTarget.y, m_r_camPosition.z - m_r_camTarget.z);
	XMVECTOR Pos_Tar_Vec = XMLoadFloat3(&Pos_Tar);
	Pos_Tar_Vec = XMVector3Normalize(Pos_Tar_Vec);

	r_camUp = XMVector3Normalize(XMVector3Cross(Pos_Tar_Vec, r_camRight));

	m_reflectionViewMatrix = XMMatrixLookAtLH(r_camPosition, r_camTarget, r_camUp);
	return;
}

void CameraClass::RenderFreeLookCam(float camYaw, float camPitch, float moveLeftRight, float moveBackForward,
	XMVECTOR DefaultForward, XMVECTOR DefalutRight) {
	XMMATRIX camRotationMatrix;

	camRotationMatrix = XMMatrixRotationRollPitchYaw(-camPitch, camYaw, 0);
	camTarget = XMVector3TransformCoord(-DefaultForward, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	camRight = XMVector3TransformCoord(-DefalutRight, camRotationMatrix);
	camForward = XMVector3TransformCoord(-DefaultForward, camRotationMatrix);
	camUp = XMVector3Cross(camForward, camRight);

	camPosition += moveLeftRight * camRight;
	camPosition += moveBackForward * camForward;

	camTarget = camPosition + camTarget;

	m_viewMatrix = XMMatrixLookAtLH(camPosition, camTarget, camUp);
}

void CameraClass::RenderFreeLookCamReflection(float camYaw, float camPitch, float moveLeftRight, float moveBackForward,
	XMVECTOR DefaultForward, XMVECTOR DefalutRight, float height, float camHeight) {

	XMMATRIX camRotationMatrix;

	camRotationMatrix = XMMatrixRotationRollPitchYaw(+camPitch, camYaw, 0);
	r_camTarget = XMVector3TransformCoord(-DefaultForward, camRotationMatrix);

	r_camTarget = XMVector3Normalize(r_camTarget);

	r_camRight = XMVector3TransformCoord(-DefalutRight, camRotationMatrix);
	r_camForward = XMVector3TransformCoord(-DefaultForward, camRotationMatrix);
	r_camUp = XMVector3Cross(r_camForward, r_camRight);

	r_camPosition += moveLeftRight * r_camRight; 
	r_camPosition += moveBackForward * r_camForward;

	r_camTarget = r_camPosition + r_camTarget;

	m_reflectionViewMatrix = XMMatrixLookAtLH(r_camPosition, r_camTarget, r_camUp);
}


void CameraClass::GetReflectionViewMatrix(XMMATRIX& m_rViewMatrix)
{
	m_rViewMatrix = m_reflectionViewMatrix;
	return;
}


void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

XMVECTOR CameraClass::GetLookDir()
{
	return camForward;
}

void CameraClass::setCollider()//콜라이더 설정 추가
{
	m_collider->Mx = XMVectorGetX(camPosition) + sizX / 2;
	m_collider->mx = XMVectorGetX(camPosition) - sizX / 2;
	m_collider->My = XMVectorGetY(camPosition) + sizY / 2;
	m_collider->my = XMVectorGetY(camPosition) - sizY / 2;
	m_collider->Mz = XMVectorGetZ(camPosition) + sizZ / 2;
	m_collider->mz = XMVectorGetZ(camPosition) - sizZ / 2;
}

void CameraClass::setSize(float x, float y, float z)//카메라 사이즈 설정 추가
{
	sizX = x; sizY = y; sizZ = z;
}

bool CameraClass::checkCollider(float max, float mix, float may, float miy, float maz, float miz)//충돌 체크 함수 추가, 충돌 시 트루, 그 외 펄스
{
	if ((mix <= m_collider->Mx && m_collider->Mx <= max) || (mix <= m_collider->mx && m_collider->mx <= max))
	{
		if ((miy <= m_collider->My && m_collider->My <= may) || (miy <= m_collider->my && m_collider->my <= may))
		{
			if ((miz <= m_collider->Mz && m_collider->Mz <= maz) || (miz <= m_collider->mz && m_collider->mz <= maz))
			{
				return true;
			}
		}
	}

	return false;
}

void CameraClass::Shutdown()//릴리즈용 셧다운 함수 추가
{
	if (m_collider)
	{
		delete m_collider;
		m_collider = 0;
	}
}