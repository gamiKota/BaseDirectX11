
struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 lightPos : TEXCOORD1;
};

Texture2D tex : register(t0);
Texture2D shadow : register(t1);
SamplerState samp : register(s0);

float4 main(PS_IN PIN) : SV_Target
{
	float4 color = tex.Sample(samp, PIN.uv);
	
	// �������猩�����W��UV�l�Ŏ擾
	float2 shadowUV = PIN.lightPos.xy / PIN.lightPos.w;
	// �[�x�l���擾
	float depth = PIN.lightPos.z / PIN.lightPos.w;
	// �e�N�X�`���̐[�x�l���擾
	float shadowDepth = shadow.Sample(samp, shadowUV).r;

	// �`�悷��[�x�l�̕����傫��(�J�������牓��)�Ȃ�e�ɂȂ�
	// �e�N�X�`���̐[�x�l�ƕ`��̐[�x�l�������悤�ȋ����̏ꍇ�A
	// �v�Z�̌덷�ŉe�ɂȂ�Ȃ��������e�Ƃ݂Ȃ����(�V���h�E�A�N�l)�B
	// �덷���l�����Đ[�x�l��␳���Ă�����(�o�C�A�X)
	if (depth > shadowDepth + 0.001f) {
		color.rgb *= 0.3f;
	}

	return color;
}