// �J���� [Camera.h]
#pragma once
#include "Component.h"

class CCamera : public Component
{
protected:
	XMFLOAT3		m_vEye;		// ���_���W
	XMFLOAT3		m_vLook;	// �����_���W
	XMFLOAT3		m_vUp;		// ����x�N�g��
	XMFLOAT4X4		m_mView;	// �r���[�ϊ�
	XMFLOAT4X4		m_mProj;	// �ˉe�ϊ�
	float			m_fFOVY;	// ����p
	float			m_fAspect;	// �A�X�y�N�g��
	float			m_fNearZ;	// �O���N���b�v����
	float			m_fFarZ;	// ����N���b�v����

	static XMFLOAT3	m_vNowEye;	// ���݂̎��_
	static XMFLOAT3	m_vNowLook;	// ���݂̒����_
	static XMFLOAT3	m_vNowUp;	// ���݂̏���x�N�g��

private:
	static CCamera*	m_pCamera;	// ���ݗL���ȃJ����

public:
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	//virtual void Draw();

	XMFLOAT4X4& GetView() { return m_mView; }
	XMFLOAT4X4& GetProj() { return m_mProj; }
	XMFLOAT3& GetEye() { return m_vEye; }
	XMFLOAT3& GetLook() { return m_vLook; }
	void SetLook(XMFLOAT3 vLook) { m_vLook = vLook; }
	static void Set(CCamera* pCamera)
		{ m_pCamera = pCamera; }
	static CCamera* Get() { return m_pCamera; }
};

