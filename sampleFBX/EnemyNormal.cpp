#include "EnemyNormal.h"
#include "GameObject.h"
#include "Status.h"
#include "Collision.h"
#include "Transform.h"
#include "EnemyState.h"
#include "Rigidbody.h"
#include "AI.h"
#include "PlayerState.h"
#include "System.h"


/*
AI
‹ß‚Ã‚©‚ê‚½‚ç—£‚ê‚é
‚»‚êˆÈŠO‚È‚ç•s’èŠú‚É’e‚¤‚Á‚Ä
ƒ‰ƒ“ƒ_ƒ€”•b‚²‚Æ‚Éƒ‰ƒ“ƒ_ƒ€‚ÉˆÚ“®(XŽ²AYŽ²A‚³‚ç‚É—£‚ê‚éˆÚ“®)
*/


enum class NoramalEnemyAI {
	Idol,
	Move,
	Attack,
};


// ’âŽ~Žž‚Ìs“®
void Normal_AI_Idol(AI* ai, EnemyState* state) {
	state->SetStateActive(ENEMY_STATE::IDOL, true);
	// ‚±‚±‚ðƒ‰ƒ“ƒ_ƒ€«‚É‚·‚é(’âŽ~ŽžŠÔ)
}
// “®ìŽž‚Ìs“®
void Normal_AI_Move(AI* ai, EnemyState* state) {
	float len = float3::Length(state->m_transform->m_position, state->GetTarget()->m_transform->m_position);
	GameObject* player = GameObject::FindGameObjectWithTag("Player");
	// ƒvƒŒƒCƒ„[‚É‘Î‚·‚és“®
	if (state->GetState<EnemyState::TargetOn>()->GetTarget() == player) {
		if (len <= 1000.f) {	// ‹ß‚¢ê‡
			state->SetStateActive(ENEMY_STATE::MOVE, true);
			state->GetState<EnemyState::Move>()->m_movement = float3(0.f, 0.f, -0.5f);
		}
		else {	// \•ª‚È‹——£‚ª‚Æ‚ê‚Ä‚éê‡
			if (player->GetComponent<PlayerState>()->GetTarget() == ai->m_gameObject) {
				ai->StartUp(3.f, true)->OnStart([s = state] {
					int move = rand() % 2;
					s->SetStateActive(ENEMY_STATE::MOVE, true);
					s->GetState<EnemyState::Move>()->m_movement = float3(move == 0 ? -0.4f : 0.4f, 0.f, 0.f);
				});
			}
			else {
				state->SetStateActive(ENEMY_STATE::IDOL, true);
			}
		}
	}
}
// UŒ‚Žž‚Ìs“®
void Normal_AI_Attack(AI* ai, EnemyState* state, Status* status) {
	state->SetStateActive(ENEMY_STATE::ATTACK_BULLET, true);
	status->m_bulletTime.InitData();
}


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
	//m_state->GetState<EnemyState::Move>()->m_movement.x = 1.f;
	// UŒ‚‘ÎÛ
	m_state->SetStateActive(ENEMY_STATE::TARGET_ON, true);
	m_state->GetState<EnemyState::TargetOn>()->SetTarget(GameObject::Find("Player"));
	m_state->GetState<EnemyState::TargetOn>()->SetMaxAngle(2.f);

	// AIƒe[ƒuƒ‹
	m_ai->m_table[(int)NoramalEnemyAI::Idol] = [sub = this] { Normal_AI_Idol(sub->m_ai, sub->m_state); };
	m_ai->m_table[(int)NoramalEnemyAI::Move] = [sub = this] { Normal_AI_Move(sub->m_ai, sub->m_state); };
	m_ai->m_table[(int)NoramalEnemyAI::Attack] = [sub = this] { Normal_AI_Attack(sub->m_ai, sub->m_state, sub->m_status); };
}

void EnemyNormal::Update() {
	// “G‚Ì‹¤’Êˆ—
	Enemy::Update();

	//--- ŽËŒ‚
	if (m_status->m_bulletTime.data >= m_status->m_bulletTime.max) {
		m_ai->m_table[(int)NoramalEnemyAI::Attack]();
	}
	//--- ˆÚ“®
	m_ai->m_table[(int)NoramalEnemyAI::Move]();
}

// EOF