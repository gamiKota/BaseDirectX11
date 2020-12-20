/**
 * @file Drop.cpp
 */


/**
 * @include
 */
#include "Drop.h"
#include "Frame.h"
#include "GameObject.h"
#include "GameObjectMesh.h"
#include "WaterSurface.h"
#include "Transform.h"
#include "Mesh.h"
#include "CollisionMesh.h"
#include "System.h"


static const float GravityAcceleration = 9.8f;


void Drop::Start() {
	m_time = 0.f;
	m_influence = 20.f;	// ドメインシェーダに与えるパラメータ(振幅)
	m_isCollsion = false;
}


void Drop::Update() {

	GameObjectMesh* mesh = dynamic_cast<GameObjectMesh*>(m_gameObject);
	WaterSurface* waterSurface = dynamic_cast<WaterSurface*>(GameObject::Find("WaterSurface"));


	if (m_isCollsion) {
		m_influence -= 0.1f;
		if (m_influence <= 0.f) {
			m_influence = 0;
		}
	}
	else {
		float a = GravityAcceleration * (m_time * m_time) * 0.5f;
		m_transform->m_position.y -= a;
	}

	//if (m_transform->m_position.y <= 0.f) {
	//	// 取り合えず波と当たった判定
	//	// どの波に当たったのかも判定したい
	//	m_isCollsion = true;
	//	m_influence -= 0.1f;
	//	if (m_influence <= 0.f) {
	//		m_influence = 0;
	//	}
	//}

	if (waterSurface != nullptr && mesh != nullptr) {
		if (CollisionMesh::isMesh2Mesh(waterSurface->m_transform, mesh->m_transform)) {
			m_isCollsion = true;
			mesh->m_mesh.isDraw = false;
		}
	}

	m_time += Frame::GetInstance().GetDeltaTime();
}


void Drop::Uninit() {

}


void Drop::OnCollision(GameObject* obj) {

}


// EOF