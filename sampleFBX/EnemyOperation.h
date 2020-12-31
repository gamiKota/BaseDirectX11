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
 * @forward declaration class
 */
class EnemyState;

struct StatusVal {
	float data;	//!< データ
	float min;	//!< データが扱う最小値
	float max;	//!< データが扱う最大値
	
	StatusVal() {
		data = min = max = 0.f;
	}
	StatusVal(float _data, float _min, float _max) {
		data = _data;
		min = _min;
		max = _max;
	}

};


/**
 * @class EnemyOperation : inheritance StateMachine<template T>
 * @brief 実質的なAIの部分
 */
class EnemyOperation : public Component {
public:
	EnemyState* m_stateMachine;	//!< 状態管理クラス
	StatusVal m_bullet;			//!< 射撃時間
	StatusVal m_targetDist;		//!< 現在ターゲットにしてるオブジェクトとの距離
	float3 m_movement;			//!< 移動量

public:
	void Awake();
	void Start();
	void Update();
};


// EOF