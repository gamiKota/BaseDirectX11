// �X�J�C�h�[��/�X�J�C�{�b�N�X [Sky.cpp]
#include "Sky.h"
#include "Camera.h"
#include "D3DClass.h"
#include "GameObject.h"
#include "ModelManager.h"
#include "System.h"


// �R���X�g���N�^
CSky::CSky() {

}

// �f�X�g���N�^
CSky::~CSky() {
}


void CSky::Awake() {

}

// ������
void CSky::Start() {
}

// �I������
void CSky::Uninit() {
}

// �X�V
void CSky::Update() {
	ModelManager::GetInstance().Update(E_MODEL_MISSILE);
}




void CSky::SetPos(XMFLOAT3 pos) {
	m_transform->m_position = pos;
}