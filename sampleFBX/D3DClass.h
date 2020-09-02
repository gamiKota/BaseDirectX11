#pragma once


//*****************************************************************************
// �����J�[
//*****************************************************************************
#pragma comment(lib, "d3d11")


//*****************************************************************************
// �C���N���[�h��
//*****************************************************************************
#include <d3d11.h>
#include <DirectXMath.h>
#include "Singleton.h"


// �{���̓w�b�_�ɏ����Ȃ������ǂ�
using namespace DirectX;


//*****************************************************************************
// �}�N���E�萔��`
//*****************************************************************************
#define	NUM_VERTEX		(4)					// ���_��
#define	NUM_POLYGON		(2)					// �|���S����


enum EBlendState {
	BS_NONE = 0,							// ��������������
	BS_ALPHABLEND,							// ����������
	BS_ADDITIVE,							// ���Z����
	BS_SUBTRACTION,							// ���Z����

	MAX_BLENDSTATE
};

enum ECullMode {
	CULLMODE_NONE = 0,						// �J�����O���Ȃ�
	CULLMODE_CW,							// �O�ʃJ�����O
	CULLMODE_CCW,							// �w�ʃJ�����O

	MAX_CULLMODE
};


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// ���_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
typedef struct {
	XMFLOAT3 vtx;		// ���_���W
	XMFLOAT4 diffuse;	// �g�U���ˌ�
	XMFLOAT2 tex;		// �e�N�X�`�����W
} VERTEX_2D;

// ���_�t�H�[�}�b�g( ���_���W[3D] / �@���x�N�g�� / ���ˌ� / �e�N�X�`�����W )
typedef struct {
	XMFLOAT3 vtx;		// ���_���W
	XMFLOAT3 nor;		// �@���x�N�g��
	XMFLOAT4 diffuse;	// �g�U���ˌ�
	XMFLOAT2 tex;		// �e�N�X�`�����W
} VERTEX_3D;


/**
 * @class	DirectX�֘A�̏���
 * @brief	DirectX�Ɉˑ����鏈�����܂Ƃ߂��֐�
 */
class D3DClass : public Singleton<D3DClass>
{
public:

	/**
	 * @brief	������
	 * @details	�I�u�W�F�N�g�̏�����
	 */
	HRESULT Initialize(HWND hWnd, BOOL bWindow);

	/**
	 * @brief	�I������
	 * @details	�I�u�W�F�N�g�̏I������
	 */
	void Uninit();

	/**
	 * @brief	�X�V����
	 * @details	�I�u�W�F�N�g�̍X�V����
	 */
	void Update();

	/**
	 * @brief	�`�揈��
	 * @details	�I�u�W�F�N�g�̕`��
	 */
	void Draw();

	/**
	 * @brief	�f�o�C�X�̎擾
	 */
	ID3D11Device* GetDevice();

	/**
	 * @brief	�f�o�C�X �R���e�L�X�g�擾
	 */
	ID3D11DeviceContext* GetDeviceContext();

	/**
	 * @brief	�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	 */
	void SwitchingBuffer();

	/**
	 * @brief	�[�x�o�b�t�@�L����������
	 * @param	true = �[�x�o�b�t�@�L��
	 */
	void SetZBuffer(bool bEnable);

	/**
	 * @brief	�[�x�o�b�t�@�X�V�L����������
	 * @param	true = �[�x�o�b�t�@�X�V�L��
	 */
	void SetZWrite(bool bEnable);

	
	void SetBlendState(int nBlendState);

	
	void SetCullMode(int nCullMode);

private:
	ID3D11Device*				m_pDevice;						//!< �f�o�C�X
	ID3D11DeviceContext*		m_pDeviceContext;				//!< �f�o�C�X �R���e�L�X�g
	IDXGISwapChain*				m_pSwapChain;					//!< �X���b�v�`�F�[��
	ID3D11RenderTargetView*		m_pRenderTargetView;			//!< �t���[���o�b�t�@
	ID3D11Texture2D*			m_pDepthStencilTexture;			//!< Z�o�b�t�@�p������
	ID3D11DepthStencilView*		m_pDepthStencilView;			//!< Z�o�b�t�@
	UINT						m_uSyncInterval;				//!< �������� (0:��, 1:�L)
	ID3D11RasterizerState*		m_pRs[MAX_CULLMODE];			//!< ���X�^���C�U �X�e�[�g
	ID3D11BlendState*			m_pBlendState[MAX_BLENDSTATE];	//!< �u�����h �X�e�[�g
	ID3D11DepthStencilState*	m_pDSS[2];						//!< Z�o�b�t�@�X�V���Ȃ�

	/**
	 * @brief	�o�b�N�o�b�t�@�̐���
	 */
	HRESULT CreateBackBuffer(void);

	/**
	 * @brief	�o�b�N�o�b�t�@���
	 */
	void ReleaseBackBuffer();
};

// EOF