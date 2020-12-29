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
	// ó‘Ô‚Ì’Ç‰Á
	StateMachine::AddState(new PlayerState::Idol(this));
	StateMachine::AddState(new PlayerState::Move(this), true);
}



void PlayerState::Idol::Start() {
	// ‰½‚à‚µ‚È‚¢
}


void PlayerState::Idol::Update() {
	// ‰½‚à‚µ‚È‚¢
	PrintDebugProc("Idol::Update\n");
	main->m_transform->m_position = float3();
}


void PlayerState::Idol::OnDestoy() {
	// ‰½‚à‚µ‚È‚¢
}


void PlayerState::Move::Start() {
	// ‰½‚à‚µ‚È‚¢
}


void PlayerState::Move::Update() {
	PrintDebugProc("Move::Update\n");
	//main->m_transform->m_position = float3();
}


void PlayerState::Move::OnDestoy() {
	// ‰½‚à‚µ‚È‚¢
}


// EOF