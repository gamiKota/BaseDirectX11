/**
 * @file Drop.cpp
 */


/**
 * @include
 */
#include "Drop.h"
#include "Frame.h"
#include "Transform.h"
#include "System.h"


static const float GravityAcceleration = 9.8f;


void Drop::Start() {
	m_time = 0.f;
	m_influence = 20.f;	// ドメインシェーダに与えるパラメータ(振幅)
	m_isCollsion = false;
}


void Drop::Update() {

	float a = GravityAcceleration * (m_time * m_time) * 0.5f;
	m_transform->m_position.y -= a;
	if (m_transform->m_position.y <= 0.f) {
		// 取り合えず波と当たった判定
		m_isCollsion = true;
		m_influence -= 0.1f;
		if (m_influence <= 0.f) {
			m_influence = 0;
		}
	}
	//m_posX = 0.5f * GravityAcceleration * (m_nTime * m_nTime);
	// posY = 0.98f * (g_nTime * g_nTime) / 2.f;

	m_time += Frame::GetInstance().GetDeltaTime();
}


void Drop::Uninit() {

}


void Drop::OnCollision(GameObject* obj) {

}


// EOF