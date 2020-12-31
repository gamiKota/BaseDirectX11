/**
 * @file PlayerOperation.h
 * @brief プレイヤー操作状態の依頼をプレイヤーステートマシンに送るコンポーネント
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
class PlayerState;


/**
 * @class PlayerOperation : inheritance StateMachine<template T>
 */
class PlayerOperation : public Component {
private:
	PlayerState* m_stateMachine;	//!< 状態管理クラス

public:
	void Start();
	void Update();
};


// EOF