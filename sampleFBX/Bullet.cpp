/**
 * @file Bullet.cpp
 */


/**
 * @include
 */
#include "Bullet.h"
#include "GameObject.h"
#include "GameObjectMesh.h"
#include "SceneManager.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "PlayerMgr.h"
#include "Status.h"
#include "Score.h"
#include "Explosion.h"
#include "Frame.h"
#include "System.h"


/**
 * @constant
 */
static const float	SPEED		= 50.0f;	// 速さ
static const char	LIFE_TIME	= 3;		// 生存時間


void Bullet::Start() {
	// Characterクラスの初期化
	Character::Init();

	// 初期化
	m_nLife = 0.f;
	m_status->m_HP = 10.f;
	m_status->m_AttakPower = 100.f;
	//m_status->m_HitStop = 5.f * Frame::GetInstance().GetFrame();
	m_collider->m_weight = E_WEIGHT::_1;
	m_collision->m_selfTag.push_back("Area");
	m_collision->m_vCenter = float3(0.f, 0.f, 1.f);
	m_collision->m_vScale = float3(2.5f, 2.5f, 47.f);
}


void Bullet::Update() {
	m_nLife += Frame::GetInstance().GetDeltaTime();
	if (m_nLife >= LIFE_TIME) {
		Destroy(m_gameObject);
		return;
	}
	// 前に進む
	m_transform->m_position += m_transform->m_forward * SPEED;
}


void Bullet::OnCollision(GameObject* obj) {
	Destroy(m_gameObject);
	if (obj->GetComponent<Status>() != nullptr) {
		obj->GetComponent<Status>()->m_HP -= m_status->m_AttakPower;
		//obj->GetComponent<Status>()->m_HitStop -= m_status->m_HitStop;
	}
	if (GameObject::Find("Score") != nullptr) {
		GameObject::Find("Score")->GetComponent<Score>()->AddScore(100);
	}

	// ビルボード
	GameObjectMesh* mesh = new GameObjectMesh(E_MESH_TYPE::BILLBORAD, E_TEXTURE_EXPLOSION, "Explosion", "Explosion");
	GameObject::Instantiate(mesh);
	mesh->AddComponent<Explosion>();
	mesh->m_transform->m_position = m_transform->m_position;
	mesh->m_transform->m_scale = float3(50.f, 50.f, 50.f);
}


// EOF