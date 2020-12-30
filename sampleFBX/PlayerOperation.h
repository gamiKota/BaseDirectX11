/**
 * @file PlayerOperation.h
 * @brief �v���C���[�����Ԃ̈˗����v���C���[�X�e�[�g�}�V���ɑ���R���|�[�l���g
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
 * @class PlayerMgr : inheritance StateMachine<template T>
 */
class PlayerOperation : public Component {
private:
	PlayerState* m_stateMachine;

public:
	void Start();
	void Update();
};

