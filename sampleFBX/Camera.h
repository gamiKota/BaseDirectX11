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
	DirectX::XMFLOAT4X4		m_mView;		//!< ビュー変換
	DirectX::XMFLOAT4X4		m_mProj;		//!< 射影変換
	float					m_fFovy;		//!< 視野角
	float					m_fAspect;		//!< アスペクト比
	float					m_fNearZ;		//!< 前方クリップ距離
	float					m_fFarZ;		//!< 後方クリップ距離
	Transform*				m_lookTarget;	//!< 注視点情報

private:
	static CCamera*	m_pCamera;	//!< 現在有効なカメラ

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