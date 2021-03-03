#include "SelectScene.h"
// �Q�[���I�u�W�F�N�g
#include "GameObject.h"
#include "GameObjectUI.h"
#include "GameObject3D.h"
#include "GameObjectMesh.h"
// �R���|�[�l���g
#include "Camera.h"
#include "Light.h"
#include "Sky.h"
// �V�X�e��
#include "System.h"


void SelectScene::Init() {

	// ���̃V�[���Ŏg�p����e�N�X�`���̓ǂݍ���
	TextureManager::GetInstance().Load(E_TEXTURE::E_TEXTURE_SELECT01);

	// �J����
	m_empty = new GameObject("MainCamera");
	CCamera::Set(m_empty->AddComponent<CCamera>());
	m_listObject.push_back(m_empty);

	// ���C�g(���s����)
	m_empty = new GameObject("MainLight");
	Light::Set(m_empty->AddComponent<Light>());
	m_listObject.push_back(m_empty);

	// �X�J�C�h�[��
	m_object3D = new GameObject3D(E_MODEL_SKY, "Sky", "Sky");
	m_object3D->AddComponent<SkyDome>();
	m_object3D->m_isLight = false;
	m_listObject.push_back(m_object3D);

	// �r�ō�탍�S
	m_UI = new GameObjectUI(E_LAYER::UI, E_TEXTURE_SELECT01, "button01", "button");
	GameObject::Instantiate(m_UI, float3(), Quaternion::identity, float3(200.f, 70.f, 0.f));

	// ���O�͍Ō�
	Scene::Init();
}


// EOF