/**
 * @file WaterSurfaceConstHS.hlsl
 */


 // ���_�V�F�[�_�[�̏o�̓p�����[�^
struct VS_OUT
{
	float3 pos    : POSITION;
	float2 texel  : TEXCOORD0;
};

// �n���V�F�[�_�[�̃p�b�`�萔�t�F�[�Y�p�̏o�̓p�����[�^
struct CONSTANT_HS_OUT
{
	float Edges[4] : SV_TessFactor;        // �p�b�`�̃G�b�W�̃e�b�Z���[�V�����W��
	float Inside[2] : SV_InsideTessFactor; // �p�b�`�����̃e�b�Z���[�V�����W��
};

// �n���V�F�[�_�[�̃R���g���[�� �|�C���g �t�F�[�Y�p�̏o�̓p�����[�^
struct HS_OUT
{
	float3 pos   : POSITION;
	float2 texel : TEXCOORD0;
};

cbuffer Param : register(b1)
{
	float cbEdgeFactor;		//�O�p�`�̕ӂ̕����ʂ̎w��
	float cbInsideFactor;	//�O�p�`�̓����̕����ʂ̎w��
};


// �p�b�`�萔�֐��̒�`
#define NUM_CONTROL_POINTS 4


// *****************************************************************************************
// �n���V�F�[�_�[�̃p�b�`�萔�t�F�[�Y
// *****************************************************************************************
CONSTANT_HS_OUT ConstantsHS_Main(InputPatch<VS_OUT, 4> ip, uint PatchID : SV_PrimitiveID)
{
	CONSTANT_HS_OUT Out;

	// �萔�o�b�t�@�̒l�����̂܂ܓn��
	Out.Edges[0] = Out.Edges[1] = Out.Edges[2] = Out.Edges[3] = cbEdgeFactor;  // �p�b�`�̃G�b�W�̃e�b�Z���[�V�����W��
	Out.Inside[0] = cbInsideFactor;  // �p�b�`�����̉����@�̃e�b�Z���[�V�����W��
	Out.Inside[1] = cbInsideFactor;  // �p�b�`�����̏c���@�̃e�b�Z���[�V�����W��

	return Out;
}


// *****************************************************************************************
// �n���V�F�[�_�[�̃R���g���[�� �|�C���g �t�F�[�Y
// *****************************************************************************************
[domain("quad")]							// �e�b�Z���[�g���郁�b�V���̌`����w�肷��
[partitioning("integer")]					// �p�b�`�̕����Ɏg�p����A���S���Y�����w�肷��
[outputtopology("triangle_ccw")]			// ���b�V���̏o�͕��@���w�肷��
[outputcontrolpoints(4)]					// �n���V�F�[�_�[�̃R���g���[�� �|�C���g �t�F�[�Y���R�[��������
[patchconstantfunc("ConstantsHS_Main")]     // �Ή�����n���V�F�[�_�[�̃p�b�`�萔�t�F�[�Y�̃��C���֐�
HS_OUT main( InputPatch<VS_OUT, 4> In, uint i : SV_OutputControlPointID, uint PatchID : SV_PrimitiveID )
{
	HS_OUT Out;
	
	// ���̂܂ܓn��
	Out.pos = In[i].pos;
	Out.texel = In[i].texel;
	return Out;
}


// EOF