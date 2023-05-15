#include "BasicShaderHeader.hlsli"

//�T���v���[��UV���W���w�肵�āA�e�N�X�`���̐F�����o��
Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 PSmain(VSOutput input) : SV_TARGET
{
    /////For texture mapping only
    //return float4(tex.Sample(smp, input.uv));
    //return float4(input.normal, 1); // RGB�����ꂼ��@����XYZ�AA��1�ŏo��

    /////For diffuse calculation
    //float3 light = normalize(float3(1, -1, 1)); // �E���� �����̃��C�g
    //float brightness = dot(-light, input.normal); // �����ւ̃x�N�g���Ɩ@���x�N�g���̓���
    //return float4(brightness, brightness, brightness, 1); // �P�x��RGB�ɑ�����ďo��

    /////Adding Ambient
    float4 texcolor = float4(tex.Sample(smp,input.uv));
    float3 light = normalize(float3(1, -1, 1)); // �E���� �����̃��C�g
    float diffuse = saturate(dot(-light, input.normal)); // diffuse��[0,1]�͈̔͂�Clamp����
    float brightness = diffuse + 0.3f; // �A���r�G���g����0.3�Ƃ��Čv�Z
    return float4(texcolor.rgb * brightness, texcolor.a);//Mapping with texture
    //return float4(brightness, brightness, brightness, 1); // �P�x��RGB�ɑ�����ďo��
    //return float4(texcolor.rgb * brightness, 1) * color;//Mapping with texture
    //return float4(0, 0, 0, 1);//All black
}

//Basic pixel shader setting

////// Entry Point //////
//float4 PSmain(float4 pos:SV_Position) : SV_TARGET
//{
//	//Original with pixel shader color
//	return float4(1.0f, 1.0f, 1.0f, 1.0f);//R G B A
//}

//To display UV color on the screen

//float4 PSmain(VSOutput input) : SV_TARGET
//{
//	return float4(input.uv, 1, 1);
//}

