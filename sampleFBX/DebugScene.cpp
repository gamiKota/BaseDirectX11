#include "DebugScene.h"
#include "GameObject3D.h"
#include "ggCamera.h"
#include "Light.h"
#include "Collision.h"
#include "System.h"

void DebugScene::Init() {
	// TPS視点カメラ
	m_empty = new GameObject("MainCamera");
	CCamera::Set(m_empty->AddComponent<ggCamera>());
	m_listObject.push_back(m_empty);

	// ライト(平行光源)
	m_empty = new GameObject("MainLight");
	Light::Set(m_empty->AddComponent<Light>());
	m_listObject.push_back(m_empty);

	// デバッグで見たいオブジェクト
	m_object3D = new GameObject3D(E_MODEL_PLAYER, "DebugObject");
	m_object3D->AddComponent<CollisionSphere>();
	m_listObject.push_back(m_object3D);
}


// EOF