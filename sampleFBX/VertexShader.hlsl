/**
 * @file VertexShader.hlsl
 */


cbuffer WorldBuffer : register(b3) {
	float4x4 world;
}

// �J�����̏��
cbuffer CameraBuffer : register(b4) {
	float4x4 view;
	float4x4 proj;
}

struct VS_IN
{
	float3 pos : POSITION0;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
};

// ���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn�����
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 wPos : TEXCOORD1;
};



VS_OUT main(VS_IN VIN)
{
	VS_OUT VOUT;
	VOUT.pos = float4(VIN.pos, 1);

	// �s��ƃx�N�g���̌v�Z
	VOUT.pos = mul(VOUT.pos, world);
	VOUT.wPos = VOUT.pos.xyz;

	// �J�������W�֕ϊ�
	VOUT.pos = mul(VOUT.pos, view);

	// �J�����̉�p
	VOUT.pos = mul(VOUT.pos, proj);

	// UV���
	VOUT.uv = VIN.uv;

	// �m�[�}���C�Y
	VOUT.normal = VIN.normal;

	return VOUT;
}

// EOF