// カメラ [Camera.h]
#pragma once
#include "Component.h"


class CCamera : public Component
{
protected:
	float3					m_vEye;		//!< 視点座標
	float3					m_vLook;	//!< 注視点座標
	float3					m_vUp;		//!< 上方ベクトル
	DirectX::XMFLOAT4X4		m_mView;	//!< ビュー変換
	DirectX::XMFLOAT4X4		m_mProj;	//!< 射影変換
	float					m_fFOVY;	//!< 視野角
	float					m_fAspect;	//!< アスペクト比
	float					m_fNearZ;	//!< 前方クリップ距離
	float					m_fFarZ;	//!< 後方クリップ距離

	static float3	m_vNowEye;	//!< 現在の視点
	static float3	m_vNowLook;	//!< 現在の注視点
	static float3	m_vNowUp;	//!< 現在の上方ベクトル

private:
	static CCamera*	m_pCamera;	//!< 現在有効なカメラ

public:
	virtual void Start();
	virtual void Uninit();
	virtual void LastUpdate();
	//virtual void Draw();

	DirectX::XMFLOAT4X4& GetView() { return m_mView; }
	DirectX::XMFLOAT4X4& GetProj() { return m_mProj; }
	float3& GetEye() { return m_vEye; }
	float3& GetLook() { return m_vLook; }
	void SetLook(float3 vLook) { m_vLook = vLook; }
	static void Set(CCamera* pCamera)
		{ m_pCamera = pCamera; }
	static CCamera* Get() { return m_pCamera; }
};

