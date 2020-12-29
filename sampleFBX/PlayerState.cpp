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
	// èÛë‘ÇÃí«â¡
	StateMachine::AddState(new PlayerState::Idol(this));
	StateMachine::AddState(new PlayerState::Move(this), true);
}



void PlayerState::Idol::Start() {
	// âΩÇ‡ÇµÇ»Ç¢
}


void PlayerState::Idol::Update() {
	// âΩÇ‡ÇµÇ»Ç¢
	PrintDebugProc("Idol::Update\n");
}


void PlayerState::Idol::OnDestoy() {
	// âΩÇ‡ÇµÇ»Ç¢
}


void PlayerState::Move::Start() {
	// âΩÇ‡ÇµÇ»Ç¢
}


void PlayerState::Move::Update() {
	PrintDebugProc("Move::Update\n");
}


void PlayerState::Move::OnDestoy() {
	// âΩÇ‡ÇµÇ»Ç¢
}


// EOF