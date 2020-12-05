//=============================================================================
//
// ���b�V�����ʏ��� [mesh.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once


#include <DirectXMath.h>
#include "D3DClass.h"
#include "Component.h"


//*****************************************************************************
// �萔��`
//*****************************************************************************
enum ePrimitiveType {
	PT_UNDEFINED = 0,
	PT_POINT,
	PT_LINE,
	PT_LINESTRIP,
	PT_TRIANGLE,
	PT_TRIANGLESTRIP,

	MAX_PRIMITIVETYPE
};

//**************************************
// �\���̒�`
//**************************************
typedef struct {
	DirectX::XMFLOAT4	Diffuse;		// Diffuse color RGBA
	DirectX::XMFLOAT4	Ambient;		// Ambient color RGB
	DirectX::XMFLOAT4	Specular;		// Specular 'shininess'
	DirectX::XMFLOAT4	Emissive;		// Emissive color RGB
	float		Power;					// Sharpness if specular highlight
} MATERIAL;


class Material : public Component {
public:
	DirectX::XMFLOAT4	m_diffuse;		//!< Diffuse color RGBA
	DirectX::XMFLOAT4	m_ambient;		//!< Ambient color RGB
	DirectX::XMFLOAT4	m_specular;		//!< Specular 'shininess'
	DirectX::XMFLOAT4	m_emissive;		//!< Emissive color RGB
	float				m_power;		//!< Sharpness if specular highlight

public:
	Material();
	void SetImGuiVal();

public:
};


typedef struct {
	ID3D11Buffer* pVertexBuffer;			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	ID3D11Buffer* pIndexBuffer;				// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

	DirectX::XMFLOAT4X4 mtxWorld;			// ���[���h�}�g���b�N�X
	DirectX::XMFLOAT3 pos;					// �|���S���\���ʒu�̒��S���W
	DirectX::XMFLOAT3 rot;					// �|���S���̉�]�p
	int nNumVertex;							// �����_��	
	int nNumIndex;							// ���C���f�b�N�X��

	DirectX::XMFLOAT4X4 mtxTexture;			// �e�N�X�`�� �}�g���b�N�X
} MESH;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMesh(void);
void UninitMesh(void);
void UpdateMesh(MESH* pMesh);
void DrawMesh(MESH* pMesh, Material* material, ID3D11ShaderResourceView* texture);
HRESULT MakeMeshVertex(ID3D11Device* pDevice, MESH* pMesh, VERTEX_3D vertexWk[], int indexWk[]);
void ReleaseMesh(MESH* pMesh);
