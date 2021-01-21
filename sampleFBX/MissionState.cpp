/*
 */


#include "MissionState.h"
#include "System.h"


void MissionState::Initialize() {
	// 状態の追加
	StateMachine::AddState(new MissionState::StartState(this), true);
	StateMachine::AddState(new MissionState::UpdateState(this));
	StateMachine::AddState(new MissionState::EndSuccess(this));
	StateMachine::AddState(new MissionState::EndFailed(this));
	StateMachine::AddState(new MissionState::CreateEnemy(this));
}


void MissionState::Update() {
	// 状態に依存しない共通処理
}


/**************************************************************************************************
 * @state ミッション開始
 *************************************************************************************************/
void MissionState::StartState::Start() {
	// もし終了処理の状態が有効だったらアクティブ値を下す
	m_parent->SetStateActive(MISSION_STATE::END_SUCCESS, false);
	m_parent->SetStateActive(MISSION_STATE::END_FAILED, false);
}

void MissionState::StartState::Update() {
	// なんか開始ロゴとか出す
	m_parent->SetStateActive(MISSION_STATE::UPDATE, true);
}

void MissionState::StartState::OnDestoy() {

}


/**************************************************************************************************
 * @state ミッション更新
 *************************************************************************************************/
void MissionState::UpdateState::Start() {
}

void MissionState::UpdateState::Update() {
}

void MissionState::UpdateState::OnDestoy() {
}


/**************************************************************************************************
 * @state ミッション終了(成功)
 *************************************************************************************************/
void MissionState::EndSuccess::Start() {
	m_parent->SetStateActive(MISSION_STATE::END_FAILED, false);
}

void MissionState::EndSuccess::Update() {
	m_parent->SetStateActive(MISSION_STATE::END_FAILED, false);
}

void MissionState::EndSuccess::OnDestoy() {
}


/**************************************************************************************************
 * @state ミッション終了(失敗)
 *************************************************************************************************/
void MissionState::EndFailed::Start() {
	m_parent->SetStateActive(MISSION_STATE::END_SUCCESS, false);
}

void MissionState::EndFailed::Update() {
	m_parent->SetStateActive(MISSION_STATE::END_SUCCESS, false);
}

void MissionState::EndFailed::OnDestoy() {
}


/**************************************************************************************************
 * @state 通常敵生成
 *************************************************************************************************/
void MissionState::CreateEnemy::Start() {
}

void MissionState::CreateEnemy::Update() {
}

void MissionState::CreateEnemy::OnDestoy() {
}

// EOF