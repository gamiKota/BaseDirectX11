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

	MAX,
};


/**
 * @class EnemyState : inheritance StateMachine<template T>
 */
class EnemyState : public StateMachine<ENEMY_STATE> {
public:
	float3 m_rotate;	//!< 回転量の保持

public:
	void Awake() { Initialize(); }
	void Update();

	// StateMachineのデストラクタに影響されない
	// メモリリークしない
	~EnemyState() {}
	void Initialize() override;


	// 静止状態
	struct Idol : public State<ENEMY_STATE> {
		EnemyState* m_main;
		Idol(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::IDOL), m_main(_main) {}
		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};

	// 移動状態
	struct Move : public State<ENEMY_STATE> {
		EnemyState* m_main;
		float3	m_movement;
		Move(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::MOVE), m_main(_main), m_movement(float3()) {}
		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};

	// ターゲットオン
	struct TargetOn : public State<ENEMY_STATE> {
		EnemyState*		m_main;
		GameObject*		m_target;	//!< ターゲット
		float			m_maxAngle;	//!< 制限角度

		TargetOn(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::TARGET_ON),
			m_main(_main), m_target(nullptr), m_maxAngle(180.f)
		{}

		void Start()		override;		// 状態移行直後
		void Update()		override;		// 状態の最中
		void OnDestoy()		override;		// 状態終了時
		void SetTarget(GameObject* target = nullptr) { m_target = target; }
		void SetMaxAngle(float maxAngle) { m_maxAngle = maxAngle; }
	};
	// ターゲットオフ
	struct TargetOff : public State<ENEMY_STATE> {
		EnemyState* m_main;
		TargetOff(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::TARGET_OFF), m_main(_main) {}
		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};

	// 射撃状態
	struct AttackBullet : public State<ENEMY_STATE> {
		EnemyState* m_main;
		AttackBullet(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::ATTACK_BULLET), m_main(_main) {}
		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};
};


// EOF