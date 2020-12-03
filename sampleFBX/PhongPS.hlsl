/**
 * @file PhongPS.hlsl
 */


 // ���_�V�F�[�_����n�����f�[�^
struct PS_IN {
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float3 normal	: NORMAL0;
	float3 wPos		: TEXCOORD1;
};


cbuffer ValueBuf : register(b5) {
	float4 value;
}


Texture2D tex : register(t0);
SamplerState samp : register(s0);


float4 main(PS_IN PIN) : SV_Target{

	float4 color;
	color = tex.Sample(samp, PIN.uv);

	// �e�̌v�Z
	// �����̃x�N�g���Ɩ@���̓��ς���Â����v�Z����
	float3 lightDir = float3(1, -1, 1);

	// �����̃x�N�g���𔽓](-�̒l���|����)���āA�����������ڂɂȂ�悤�Ɍv�Z����
	lightDir = normalize(-lightDir);
	float3 N = normalize(PIN.normal);

	// ����
	float d = dot(lightDir, N);

	// 1�`0�͈̔͂ɕ␳����
	d = d * 0.5f + 0.5f;

	// �Ƃ�Ԃ�(���˂̌v�Z)
	float3 V = PIN.wPos - value.xyz;
	// reflect... �������œn�����@���̃x�N�g������A�������̃x�N�g�����ǂ̂悤�ɔ��˂��邩�v�Z
	float3 R = reflect(V, N);
	R = normalize(R);
	float s = dot(lightDir, R);
	// �X�y�L�����[��1�`0�͈̔͂ɂȂ��l�𖳎�����
	// saturate... ������0�`1�͈̔͂Ɋۂ߂�
	s = saturate(s);
	// �Ƃ�Ԃ������͂�苭���ۗ��悤�Ɍv�Z���ʂ���Z����
	s = pow(s, 50);

	// �l�Ԃ̖ڂ́A�͂���������F�̔��ʂ��s��
	// ��������i�񂾌������ڕ��̂ɓ�����A���˂������̂��Ƃ��g�U��(Diffuse)�ƌĂ�
	// ���ӂ̕��̂ɂ������������˂��āA���̂ɓ�����A����ɔ��˂�����������(Ambient)�ƌĂ�
	// ���̂��̂��̂̌��̔��˂̂��₷����\���}�e���A��(Material)�Ƃ����p�����[�^������
	// �P���Č�������𔽎ˌ�(Specular)�ƌĂ�

	// �����̐F
	float4 diffuseLight		= float4(1, 1, 1, 1);
	// ���肩�甽�˂����F
	float4 ambientLight		= float4(0.5f, 0.5f, 0.5f, 1);
	// �P���Č�������̐F
	float4 specularLight	= float4(1, 1, 1, 1);
	// ���̂����˕Ԃ��₷����
	float4 materialDiffuse	= float4(1, 1, 1, 1);
	// ���̂����˕Ԃ��₷������
	float4 materialAmbient	= float4(1, 1, 1, 1);
	// ���̂��������ۂ���(���ˌ��̌W��)
	float4 materialSpecular = float4(50, 10, 10, 1);

	// ���}�e���A�����l�������e��
	float3 diffuse = (float3)(diffuseLight * materialDiffuse);
	float3 ambient = (float3)(ambientLight * materialAmbient);
	float3 specular = (float3)(specularLight * materialSpecular);
	float3 lambert = diffuse * d + ambient;
	float3 phong = specular * s;

	color.rgb *= lambert + phong;

	return color;
}


// EOF