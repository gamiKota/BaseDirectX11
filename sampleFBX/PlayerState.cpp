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
	// 状態の追加
	StateMachine::AddState(new PlayerState::Idol(this));
	StateMachine::AddState(new PlayerState::Move(this), true);
}



void PlayerState::Idol::Start() {
	// 何もしない
}


void PlayerState::Idol::Update() {
	// 何もしない
	PrintDebugProc("Idol::Update\n");
}


void PlayerState::Idol::OnDestoy() {
	// 何もしない
}


void PlayerState::Move::Start() {
	// 何もしない
}


void PlayerState::Move::Update() {
	PrintDebugProc("Move::Update\n");
}


void PlayerState::Move::OnDestoy() {
	// 何もしない
}


// EOF