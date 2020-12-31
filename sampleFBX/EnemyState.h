/**
 * @file EnemyState.h
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
 * @enum ENEMY_STATE
 * @brief 敵の状態列挙
 */
enum class ENEMY_STATE {
	IDOL,			// 静止
	MOVE,			// 移動
	TARGET_ON,		// ターゲットON
	TARGET_OFF,		// ターゲットOFF
	ATTACK_BULLET,	// 射撃攻撃
};


/**
 * @class EnemyState : inheritance StateMachine<template T>
 */
class EnemyState : public StateMachine<ENEMY_STATE> {
private:
	GameObject*		m_target;		//!< ターゲット
	float			m_roll;			//!< 傾き(Z軸)
	float3			m_movement;		//!< 移動ベクトル

public:
	void Awake() { Initialize(); }
	void Update();

	// StateMachineのデストラクタに影響されない
	// メモリリークしない
	~EnemyState() {}
	void Initialize() override;


	// 静止状態
	struct Idol : public State<ENEMY_STATE> {
		EnemyState* main;
		Idol(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::IDOL), main(_main) {}
		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};

	// 移動状態
	struct Move : public State<ENEMY_STATE> {
		EnemyState* main;
		Move(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::MOVE), main(_main) {}
		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};

	// ターゲットオン
	struct TargetOn : public State<ENEMY_STATE> {
		EnemyState* main;
		TargetOn(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::TARGET_ON), main(_main) {}
		void Start()		override;		// 状態移行直後
		void Update()		override;		// 状態の最中
		void OnDestoy()		override;		// 状態終了時
	};
	// ターゲットオフ
	struct TargetOff : public State<ENEMY_STATE> {
		EnemyState* main;
		TargetOff(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::TARGET_OFF), main(_main) {}
		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};

	// 射撃状態
	struct AttackBullet : public State<ENEMY_STATE> {
		EnemyState* main;
		AttackBullet(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::ATTACK_BULLET), main(_main) {}
		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};

	GameObject* GetTarget() { return m_target; }
	void SetTarget(GameObject* target = nullptr) { m_target = target; }
	void SetMovement(float3 move) { m_movement = move; }
};


// EOF