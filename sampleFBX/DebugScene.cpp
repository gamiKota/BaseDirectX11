#include "DebugScene.h"
#include "GameObject3D.h"
#include "ggCamera.h"
#include "Light.h"
#include "Collision.h"
#include "System.h"

void DebugScene::Init() {
	// TPS���_�J����
	m_empty = new GameObject("MainCamera");
	CCamera::Set(m_empty->AddComponent<ggCamera>());
	m_listObject.push_back(m_empty);

	// ���C�g(���s����)
	m_empty = new GameObject("MainLight");
	Light::Set(m_empty->AddComponent<Light>());
	m_listObject.push_back(m_empty);

	// �f�o�b�O�Ō������I�u�W�F�N�g
	m_object3D = new GameObject3D(E_MODEL_PLAYER, "DebugObject");
	m_object3D->AddComponent<CollisionSphere>();
	m_listObject.push_back(m_object3D);
}


// EOF