/**
 * @PlayerState.cpp
 */


/**
 * @include
 */
#include "PlayerState.h"
#include "Transform.h"
#include "GameObject.h"
#include "debugproc.h"
#include "System.h"



void PlayerState::Initialize() {
	// ��Ԃ̒ǉ�
	StateMachine::AddState(new PlayerState::Idol(this));
	StateMachine::AddState(new PlayerState::Move(this), true);
}



void PlayerState::Idol::Start() {
	// �������Ȃ�
}


void PlayerState::Idol::Update() {
	// �������Ȃ�
	PrintDebugProc("Idol::Update\n");
}


void PlayerState::Idol::OnDestoy() {
	// �������Ȃ�
}


void PlayerState::Move::Start() {
	// �������Ȃ�
}


void PlayerState::Move::Update() {
	PrintDebugProc("Move::Update\n");
}


void PlayerState::Move::OnDestoy() {
	// �������Ȃ�
}


// EOF