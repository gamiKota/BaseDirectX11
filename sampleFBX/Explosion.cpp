/**
 * @file Explosion.h
 */


/**
 * @include
 */
#include "Explosion.h"
#include "GameObjectMesh.h"
#include "Input.h"
#include "Camera.h"
#include "Frame.h"
#include "System.h"



void Explosion::Start() {
	// GameObjectMesh変数の設定
	GameObjectMesh* mesh = dynamic_cast<GameObjectMesh*>(m_gameObject);	//!< 親メッシュ
	mesh->m_mesh.texSize = float3(0.125f, 1.f, 1.f);
	mesh->m_mesh.texPattern = float3(0.f, 0.f, 0.f);
	mesh->m_mesh.light = false;

	// 変数の初期化
	m_time = 0.f;
}


void Explosion::Update() {
	GameObjectMesh* mesh = dynamic_cast<GameObjectMesh*>(m_gameObject);	//!< 親メッシュ

	m_time += Frame::GetInstance().GetDeltaTime();
	if (m_time >= 0.025f) {	// 秒数判定
		mesh->m_mesh.texPattern.x++;
		if (mesh->m_mesh.texPattern.x >= 8) {
			GameObject::Destroy(m_gameObject);
		}
		m_time = 0.f;
	}
}


// EOF