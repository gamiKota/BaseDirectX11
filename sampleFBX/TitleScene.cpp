/**
 * @file	GameManager.cpp
 * @brief	�Q�[���V�[���̃}�l�[�W���[
 *
 * @author	Kota Nakagami
 * @date	2019/11/09
 *
 * @version	1.00
 */


 //*****************************************************************************
 // �C���N���[�h��
 //*****************************************************************************
#include "TitleScene.h"		// ���g�̃w�b�_�[
#include "D3DClass.h"
#include "input.h"
#include "debugproc.h"
#include "GameObject.h"
#include "Camera.h"
#include "Light.h"
#include "System.h"


void TitleScene::Init() {

	// �J����
	m_empty = new GameObject;
	CCamera::Set(m_empty->AddComponent<CCamera>());
	m_listObject.push_back(m_empty);

	// ���C�g(���s����)
	m_empty = new GameObject("MainLight");
	Light::Set(m_empty->AddComponent<Light>());
	m_listObject.push_back(m_empty);

	// ���O�͍Ō�
	Scene::Init();
}


// EOF