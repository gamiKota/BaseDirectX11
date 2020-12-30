/*
 * @file PlayerOperation.cpp
 */


/**
 * @include
 */
#include "PlayerOperation.h"
#include "Component.h"
#include "GameObject.h"
#include "PlayerState.h"
#include "input.h"
#include "debugproc.h"
#include "System.h"


void PlayerOperation::Start() {
	m_stateMachine = m_gameObject->GetComponent<PlayerState>();
}


void PlayerOperation::Update() {
	// ターゲット処理
	if (Input::isTrigger('T')) {		// ロックオン
		m_stateMachine->SetStateActive(PLAYER_STATE::TARGET_ON,  true);
	}
	else if (Input::isTrigger('O')) {	// ロックオフ
		m_stateMachine->SetStateActive(PLAYER_STATE::TARGET_OFF, true);
	}

	// 移動処理
	float3 movement = float3();
	if (Input::isPress('W')) {
		movement.z +=  1;
	}
	if (Input::isPress('S')) {
		movement.z += -1;
	}
	if (Input::isPress('D')) {
		movement.x +=  1;
	}
	if (Input::isPress('A')) {
		movement.x += -1;
	}
	if (Input::isPress(VK_UP)) {
		movement.y +=  1;
	}
	if (Input::isPress(VK_DOWN)) {
		movement.y += -1;
	}
	if (movement.x == 0.f && movement.y == 0.f && movement.z == 0.f) {
		m_stateMachine->SetStateActive(PLAYER_STATE::IDOL, true);
	}
	else {
		m_stateMachine->SetStateActive(PLAYER_STATE::MOVE, true);
		m_stateMachine->SetMovement(movement);
	}

	// 射撃攻撃
	if (Input::isTrigger(VK_SPACE)) {
		m_stateMachine->SetStateActive(PLAYER_STATE::ATTACK_BULLET, true);
	}
}


// EOF