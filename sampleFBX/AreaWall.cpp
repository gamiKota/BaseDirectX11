#include "AreaWall.h"
#include "GameObject3D.h"
#include "Collision.h"
#include "Rigidbody.h"
#include "Material.h"
#include "System.h"

void AreaWall::Awake() {
	GameObject3D *obj = dynamic_cast<GameObject3D*>(m_gameObject);
	
	obj->m_material->m_ambient.w = 0.f;
	obj->m_material->m_diffuse = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	obj->m_ps = PS_AREAWALL;

	m_gameObject->AddComponent<CollisionBox>()->m_selfTag.push_back("Area");
	m_gameObject->AddComponent<Collider>()->m_weight = E_WEIGHT::_WALL;
	
	m_gameObject->GetComponent<CollisionBox>()->m_vScale = float3(1.f, 1.f, 1.f);

}

// EOF