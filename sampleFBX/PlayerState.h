/**
 * @file PlayerState.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "State.h"


/**
 * @enum PLAYER_STATE
 * @brief プレイヤー操作の状態列挙
 */
enum class PLAYER_STATE {
	IDOL,			// 静止
	MOVE,			// 移動
	TARGET_ON,		// ターゲットON
	TARGET_OFF,		// ターゲットOFF
	ATTACK_BULLET,	// 射撃攻撃
};


class GameObjectUI;


/**
 * @class PlayerState : inheritance StateMachine<template T>
 */
class PlayerState : public StateMachine<PLAYER_STATE> {
private:
	GameObject*		m_target;		//!< ターゲット(敵と相互管理)
	float3			m_rotate;		//!< 傾き
	float3			m_movement;		//!< 移動量(絶対ここにあって良い変数じゃない)
	GameObjectUI*	m_targetLogo;	//!< ターゲットロゴ
	GameObjectUI*	m_HPGauge;		//!< HPゲージ

public:
	void Awake() { Initialize(); }
	void Update();

	// StateMachineのデストラクタに影響されない
	// メモリリークしない
	~PlayerState() {}
	void Initialize() override;


	// 静止状態
	struct Idol : public State<PLAYER_STATE> {
		PlayerState* main;
		Idol(PlayerState* _main) : State<PLAYER_STATE>(PLAYER_STATE::IDOL), main(_main) {}
		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};

	// 移動状態
	struct Move : public State<PLAYER_STATE> {
		PlayerState* main;
		Move(PlayerState* _main) : State<PLAYER_STATE>(PLAYER_STATE::MOVE), main(_main) {}
		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};

	// ターゲットオン
	struct TargetOn : public State<PLAYER_STATE> {
		PlayerState* main;
		int	m_cnt;
		TargetOn(PlayerState* _main) : State<PLAYER_STATE>(PLAYER_STATE::TARGET_ON), main(_main), m_cnt(0) {}
		void Start()		override;		// 状態移行直後
		void Start(bool)	override;		// 再度呼び出し
		void Update()		override;		// 状態の最中
		void OnDestoy()		override;		// 状態終了時
	};
	// ターゲットオフ
	struct TargetOff : public State<PLAYER_STATE> {
		PlayerState* main;
		TargetOff(PlayerState* _main) : State<PLAYER_STATE>(PLAYER_STATE::TARGET_OFF), main(_main) {}
		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};

	// 射撃状態
	struct AttackBullet : public State<PLAYER_STATE> {
		PlayerState* main;
		AttackBullet(PlayerState* _main) : State<PLAYER_STATE>(PLAYER_STATE::ATTACK_BULLET), main(_main) {}
		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};


	GameObject* GetTarget() { return m_target; }
	void SetTarget(GameObject* target = nullptr) { m_target = target; }
	void SetMovement(float3 move) { m_movement = move; }
};


// EOF