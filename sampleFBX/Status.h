/**
 * @file Status.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "Component.h"


struct StatusVal {
	float data;	//!< データ
	float min;	//!< データが扱う最小値
	float max;	//!< データが扱う最大値

	StatusVal() {
		data = min = max = 0.f;
	}
	StatusVal(float _data, float _max, float _min = 0.f) {
		data = _data;
		min = _min;
		max = _max;
	}
	void InitData() {
		data = min;
	}
};


/**
 * @class Status : inheritance Component
 * @brief 各オブジェクト(ミサイルなど)に持たせる
 */
class Status : public Component {
public:	// ステータス
	float	m_HP;			//!< 体力
	float	m_AttakPower;	//!< 攻撃力

public:
	float	m_targetDist;	//!< ターゲットとの距離

	// 最大値までの加算はStatusクラスで実行
	// 値初期化は各コンポーネントで実行
public:	// 時間
	StatusVal	m_bulletTime;	//!< 弾発射管理
	StatusVal	m_deleteTime;	//!< 死亡経過時間
	StatusVal	m_hitStopTime;	//!< 被弾時間

public:	// フラグ
	bool	m_isDead;		//!< 死亡フラグ


public:
	Status();
	void Start();
	void Update();
	void SetImGuiVal();
};


// EOF