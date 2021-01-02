/**
 * @file EnemyOperation.h
 * @brief 敵の状態依頼をテートマシンに送るコンポーネント
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "Component.h"


/**
 * @class EnemyOperation : inheritance StateMachine<template T>
 * @brief 実質的なAIの部分
 */
class EnemyOperation : public Component {
public:
	//EnemyState* m_stateMachine;	//!< 状態管理クラス
	//StatusVal m_bullet;			//!< 射撃時間
	//StatusVal m_targetDist;		//!< 現在ターゲットにしてるオブジェクトとの距離
	//float3 m_movement;			//!< 移動量

public:
	void Awake();
	void Start();
	void Update();
};


// EOF