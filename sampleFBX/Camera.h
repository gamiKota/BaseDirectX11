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


/**
 * @class CCamera : inheritance Component
 */
class CCamera : public Component
{
protected:
	DirectX::XMFLOAT4X4		m_mView;		//!< �r���[�ϊ�
	DirectX::XMFLOAT4X4		m_mProj;		//!< �ˉe�ϊ�
	float					m_fFovy;		//!< ����p
	float					m_fAspect;		//!< �A�X�y�N�g��
	float					m_fNearZ;		//!< �O���N���b�v����
	float					m_fFarZ;		//!< ����N���b�v����
	Transform*				m_lookTarget;	//!< �����_���

private:
	static CCamera*	m_pCamera;	//!< ���ݗL���ȃJ����

public:
	virtual void Start();
	virtual void Uninit();
	virtual void Update();
	virtual void LastUpdate();

	DirectX::XMFLOAT4X4& GetView() { return m_mView; }
	DirectX::XMFLOAT4X4& GetProj() { return m_mProj; }

	void SetLook(Transform* transform) { m_lookTarget = transform; }

	static void Set(CCamera* pCamera) { m_pCamera = pCamera; }
	static CCamera* Get() { return m_pCamera; }
};


// EOF