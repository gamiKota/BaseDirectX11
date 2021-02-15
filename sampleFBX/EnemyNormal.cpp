#include "EnemyNormal.h"
#include "GameObject.h"
#include "Status.h"
#include "Collision.h"
#include "Transform.h"
#include "EnemyState.h"
#include "Rigidbody.h"
#include "System.h"

void EnemyNormal::Start() {
	// “G‚Ì‹¤’Êˆ—
	Enemy::Start();

	// •Ï”‚Ì‰Šú‰»
	m_collider->m_weight = E_WEIGHT::_2;
	m_collision->m_vCenter = float3(0.f, 0.f, 38.f);
	m_collision->m_vScale = float3(130.f, 45.f, 125.f);

	// ƒXƒe[ƒ^ƒX•Ï”‚Ì‰Šú‰»
	m_status->SetHP(1000.f);
	m_status->m_AttakPower = 10.f;
	m_status->m_deleteTime.max = 2.f;	// •bÁ–Å
	m_status->m_bulletTime.max = 3.f;	// ’e”­ŽËŠÔŠu

	// ó‘Ô‚Ì‰Šú‰»
	// ˆÚ“®
	m_state->SetStateActive(ENEMY_STATE::MOVE, true);
	m_state->GetState<EnemyState::Move>()->m_movement.x = 1.f;
	// UŒ‚‘ÎÛ
	m_state->SetStateActive(ENEMY_STATE::TARGET_ON, true);
	m_state->GetState<EnemyState::TargetOn>()->SetTarget(GameObject::Find("Player"));
	m_state->GetState<EnemyState::TargetOn>()->SetMaxAngle(5.f);
}

void EnemyNormal::Update() {
	// “G‚Ì‹¤’Êˆ—
	Enemy::Update();

	// ˆÚ“®

	// ŽËŒ‚UŒ‚
	if (float3::Length(m_transform->m_position,
		m_state->GetState<EnemyState::TargetOn>()->GetTarget()->m_transform->m_position) < 2000.f) {
		if (m_status->m_bulletTime.data >= m_status->m_bulletTime.max) {
			m_state->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
			m_status->m_bulletTime.InitData();
		}
	}
}

// EOF