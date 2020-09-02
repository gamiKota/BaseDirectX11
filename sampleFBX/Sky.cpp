// �X�J�C�h�[��/�X�J�C�{�b�N�X [Sky.cpp]
#include "Sky.h"
#include "Camera.h"
#include "D3DClass.h"
#include "GameObject.h"
#include "ModelManager.h"
#include "System.h"

// �}�N����`
#define PATH_MODELSKY	"data/model/sky.fbx"

// �R���X�g���N�^
CSky::CSky() {

}

// �f�X�g���N�^
CSky::~CSky() {
}


void CSky::Awake() {
	m_transform = &m_gameObject->m_transform;
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

// �`��
void CSky::Draw()
{
	ModelManager::GetInstance().Draw(E_MODEL_SKY, m_gameObject->m_transform);
}


void CSky::SetPos(XMFLOAT3 pos) {
	m_transform->_41 = pos.x;
	m_transform->_42 = pos.y;
	m_transform->_43 = pos.z;
}