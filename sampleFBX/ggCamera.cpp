/**
 * @file ggCamera.cpp
 */


#include "ggCamera.h"
#include "Graphics.h"
#include "input.h"
#include "Transform.h"
#include "GameObject.h"
#include <DirectXMath.h>
#include <Windows.h>


using namespace DirectX;


// �萔
namespace {
	const float3 g_vEye = float3(0.0f, 150.0f, -1200.0f);
	const float3 g_vLook = float3(0.0f, 0.0f, 0.0f);
	const float3 g_vUp(0.0f, 1.0f, 0.0f);
};


ggCamera::ggCamera() {
	m_fFovy = XMConvertToRadians(45);
	m_fAspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	m_fNearZ = 10.0f;
	m_fFarZ = 100000.0f;
}


void ggCamera::Awake() {
	m_transform->m_position = g_vEye;
	m_vLook = g_vLook;
	m_vUp = g_vUp;
}


void ggCamera::Start() {
}


void ggCamera::Update() {

}


void ggCamera::LastUpdate() {
	POINT mousePos = Input::GetMousePos();
	POINT mouseDist = {
		mousePos.x - m_oldMousePos.x,
		mousePos.y - m_oldMousePos.y,
	};
	DirectX::XMVECTOR vPos = DirectX::XMVectorSet(m_transform->m_position.x, m_transform->m_position.y, m_transform->m_position.z, 0);
	DirectX::XMVECTOR vLook = DirectX::XMVectorSet(m_vLook.x, m_vLook.y, m_vLook.z, 0);
	DirectX::XMVECTOR vFront = DirectX::XMVectorSubtract(vLook, vPos);
	DirectX::XMVECTOR vUp = DirectX::XMVectorSet(m_vUp.x, m_vUp.y, m_vUp.z, 0);
	DirectX::XMVECTOR vSide;
	float focus = 0.0f;
	
	DirectX::XMStoreFloat(&focus, DirectX::XMVector3Length(vFront));
	vFront = DirectX::XMVector3Normalize(vFront);
	vUp = DirectX::XMVector3Normalize(vUp);
	vSide = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vUp, vFront));
	vUp = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vFront, vSide));
	
	// ���{�^���_�u���N���b�N(������
	if (Input::IsMouseDoubleClick(VK_LBUTTON))
	{
		m_transform->m_position = g_vEye;
		m_vLook = g_vLook;
	}
	else if (mouseDist.x != 0 || mouseDist.y != 0)
	{
		// ���{�^��(�J������荞��
		if (Input::isPress(VK_LBUTTON))
		{
			// ��]��
			float angleX = 360.f * mouseDist.x / SCREEN_WIDTH;	// ��ʈ����360�x��](��ʃT�C�Y�̔�����180�x��])
			float angleY = 180.f * mouseDist.y / SCREEN_HEIGHT;	// ��ʈ����180�x��](��ʃT�C�Y�̔�����90�x��])
	
			// ��]�ʂ����ƂɁA�e���̉�]�s����v�Z
			DirectX::XMVECTOR rotPos = DirectX::XMVectorSubtract(vPos, vLook);
			DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angleX));
			DirectX::XMVECTOR rotAxis = DirectX::XMVector3TransformCoord(vSide, rotY);
			DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationAxis(rotAxis, DirectX::XMConvertToRadians(angleY));
	
			// �����_�����_�Ƃ��ĉ�]
			rotPos = DirectX::XMVector3TransformCoord(rotPos, rotY);
			rotPos = DirectX::XMVector3TransformCoord(rotPos, rotX);
			DirectX::XMStoreFloat3(&m_transform->m_position, DirectX::XMVectorAdd(rotPos, vLook));
		}
		// ���{�^��(�J�����ړ�
		else if (Input::isPress(VK_MBUTTON))
		{
			float rate = focus / m_fFarZ;
			float aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
			// ���A�A����B�Ƃ���O�p�`�ɂ��� tan�� = A / B�����藧��
			// ��L�������ƂɊ���o�������i�̈ړ��ʂƁA�t�H�[�J�X�ʒu / ���i �̔䗦����A�J�����̈ړ��ʂ����߂�
			float farMoveX = tanf(DirectX::XM_PI / 6.0f) * m_fFarZ * -mouseDist.x * aspect / (SCREEN_WIDTH * 0.5f);
			float farMoveY = tanf(DirectX::XM_PI / 6.0f) * m_fFarZ * mouseDist.y / (SCREEN_HEIGHT * 0.5f);
			// �p���s������ƂɃJ�������ړ�
			DirectX::XMVECTOR vMove = DirectX::XMVectorAdd(DirectX::XMVectorScale(vSide, farMoveX * rate), DirectX::XMVectorScale(vUp, farMoveY * rate));
			DirectX::XMStoreFloat3(&m_transform->m_position, DirectX::XMVectorAdd(vPos, vMove));
			DirectX::XMStoreFloat3(&m_vLook, DirectX::XMVectorAdd(vLook, vMove));
		}
		// �E�{�^��(�����ύX
		else if (Input::isPress(VK_RBUTTON))
		{
			DirectX::XMStoreFloat3(&m_transform->m_position, DirectX::XMVectorAdd(vPos, DirectX::XMVectorScale(vFront, focus * mouseDist.y * 0.01f)));
		}
	}
	else
	{
		// �X�N���[��
		int wheel = Input::GetMouseWheel();
		if (wheel != 0)
		{
			DirectX::XMStoreFloat3(&m_transform->m_position, DirectX::XMVectorAdd(vPos, DirectX::XMVectorScale(vFront, focus * wheel * 0.1f)));
		}
	}
	
	m_oldMousePos = mousePos;

	GameObject* sky = GameObject::FindGameObjectWithTag("Sky");
	if (sky != nullptr) {
		sky->m_transform->m_position = m_transform->m_position;
	}


	CCamera::LastUpdate();
}


// EOF