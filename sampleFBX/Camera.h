/**
 * @file Camera.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "Component.h"


#define CAMERA_LOOK_LENG (500.f)
#define CAMERA_LOOK_POS (m_transform->m_forward * CAMERA_LOOK_LENG)


/**
 * @class CCamera : inheritance Component
 */
class CCamera : public Component
{
protected:
	//float3					m_vLook;	//!< �����_���W
	//float3					m_vUp;		//!< ����x�N�g��
	DirectX::XMFLOAT4X4		m_mView;	//!< �r���[�ϊ�
	DirectX::XMFLOAT4X4		m_mProj;	//!< �ˉe�ϊ�
	float					m_fFovy;	//!< ����p
	float					m_fAspect;	//!< �A�X�y�N�g��
	float					m_fNearZ;	//!< �O���N���b�v����
	float					m_fFarZ;	//!< ����N���b�v����

	static float3	m_vNowEye;	//!< ���݂̎��_
	static float3	m_vNowLook;	//!< ���݂̒����_
	static float3	m_vNowUp;	//!< ���݂̏���x�N�g��

private:
	static CCamera*	m_pCamera;	//!< ���ݗL���ȃJ����

public:
	virtual void Start();
	virtual void Uninit();
	virtual void Update();
	virtual void LastUpdate();
	//virtual void Draw();

	DirectX::XMFLOAT4X4& GetView() { return m_mView; }
	DirectX::XMFLOAT4X4& GetProj() { return m_mProj; }

	//float3& GetLook() { return m_vLook; }
	//void SetLook(float3 vLook) { m_vLook = vLook; }

	void SetLook(Transform* transform) {}

	static void Set(CCamera* pCamera) { m_pCamera = pCamera; }
	static CCamera* Get() { return m_pCamera; }
};


// EOF