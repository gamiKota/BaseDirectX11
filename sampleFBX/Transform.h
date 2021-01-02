/**
 * @file Transform.h
 */


#pragma once


#include <list>
#include "Component.h"
#include "Data.h"


class Tween;
class GameObject;

/**
 * @class Transform : inheritance Component
 */
class Transform : public Component {
public:
	float3		m_position;		//!< 座標
	Quaternion	m_rotate;		//!< 回転
	float3		m_scale;		//!< 拡縮
	float3		m_forward;		//!< 前方向
	float3		m_right;		//!< 右方向
	float3		m_up;			//!< 上方向

	Tween*		m_tween;		//!< tween
	
	GameObject* m_Parent;	//!< 親オブジェクト

private:
	DirectX::XMFLOAT4X4 m_world;		//!< ワールド行列


public:
	Transform();
	void Uninit();
	void Update();
	void LastUpdate();
	void SetImGuiVal();

	Tween* DOMove(float3 position, float time = 0.f);

	void LookAt(Transform* target, float angle = 0.f);

	DirectX::XMFLOAT4X4 GetMatrixBillboard();

	DirectX::XMFLOAT4X4& GetMatrix() {
		return m_world;
	}
};

