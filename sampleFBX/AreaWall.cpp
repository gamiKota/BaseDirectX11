#include "AreaWall.h"
#include "GameObject3D.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "Material.h"
#include "System.h"

void AreaWall::Awake() {
	GameObject3D *obj = dynamic_cast<GameObject3D*>(m_gameObject);
	
	obj->m_material->m_ambient.w = 0.f;
	obj->m_material->m_diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 0.f);

	m_gameObject->AddComponent<Collision>()->m_selfTag.push_back("Area");
	m_gameObject->AddComponent<Rigidbody>()->m_weight = E_WEIGHT::_WALL;
}

// EOF