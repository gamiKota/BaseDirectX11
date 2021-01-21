/**
 * @file Collision.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "Component.h"
#include "ModelManager.h"
#include <d3d11.h>
#include <list>


/**
 * @class Collision : inheritance Component
 */
class Collision : public Component {
public:
	E_MODEL				m_model;
	DirectX::XMFLOAT4	m_color;
	float3				m_vCenter;		//!< ���E�{�b�N�X���S���W
	float3				m_vBBox;		//!< ���E�{�b�N�X �T�C�Y
	float3				m_vPosBBox;		//!< ���E�{�b�N�X���S���W(���[���h���)
	bool				m_bHit;			//!< �Փ˗L��

	std::list<std::string> m_selfTag;	//!< �Z���t�^�O

	ID3D11Buffer* m_pVertexBuffer;			//!< ���_�o�b�t�@
	ID3D11Buffer* m_pIndexBuffer;			//!< �C���f�b�N�X�o�b�t�@

private:
	bool m_isInit;

public:
	Collision();
	~Collision();

	void Awake();
	void Start();
	void Uninit();
	void Update();
	void LastUpdate();
	void SetImGuiVal();
	void OnCollision(GameObject* obj);

	void DebugDraw();


	DirectX::XMFLOAT4X4 GetWorld();
	inline void SetHit() { m_bHit = true; }

	static bool AABB(Collision obj1, Collision obj2);
	static bool OBB(Collision obj1, Collision obj2);

private:
	void Init(E_MODEL model);
};


//void InitCollisionShader();


// EOF