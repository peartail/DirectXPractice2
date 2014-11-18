cbuffer MatrixBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer ScreenSizeBuffer : register(b1)
{
	float screenHeight;
	float3 padding;
};

Texture2D shaderTex;
SamplerState sampleType;

struct VInputT
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct PInputT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float2 texc1 : TEXCOORD1;
	float2 texc2 : TEXCOORD2;
	float2 texc3 : TEXCOORD3;
	float2 texc4 : TEXCOORD4;
	float2 texc5 : TEXCOORD5;
	float2 texc6 : TEXCOORD6;
	float2 texc7 : TEXCOORD7;
	float2 texc8 : TEXCOORD8;
	float2 texc9 : TEXCOORD9;
};

PInputT VS(VInputT i)
{
	PInputT o;
	float texSize;

	i.pos.w = 1.0f;

	o.pos = mul(i.pos, world);
	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);

	o.tex = i.tex;

	texSize = 1.f / screenHeight;

	o.texc1 = i.tex + float2(0.f, texSize * -4.0f);
	o.texc2 = i.tex + float2(0.f, texSize * -3.0f);
	o.texc3 = i.tex + float2(0.f, texSize * -2.0f);
	o.texc4 = i.tex + float2(0.f, texSize * -1.0f);
	o.texc5 = i.tex + float2(0.f, texSize * 0.0f);
	o.texc6 = i.tex + float2(0.f, texSize * 1.0f);
	o.texc7 = i.tex + float2(0.f, texSize * 2.0f);
	o.texc8 = i.tex + float2(0.f, texSize * 3.0f);
	o.texc9 = i.tex + float2(0.f,texSize * 4.0f);

	return o;
}

float4 PS(PInputT i) : SV_TARGET
{
	float w0, w1, w2, w3, w4;
	float normal;
	float4 color;

	w0 = 1;
	w1 = 0.9f;
	w2 = 0.55;
	w3 = 0.18;
	w4 = 0.1;

	normal = (w0 + 2.f *(w1 + w2 + w3 + w4));

	w0 = w0 / normal;
	w1 = w1 / normal;
	w2 = w2 / normal;
	w3 = w3 / normal;
	w4 = w4 / normal;

	color = float4(0, 0, 0, 0);

	color += shaderTex.Sample(sampleType, i.texc1)*w4;
	color += shaderTex.Sample(sampleType, i.texc2)*w3;
	color += shaderTex.Sample(sampleType, i.texc3)*w2;
	color += shaderTex.Sample(sampleType, i.texc4)*w1;
	color += shaderTex.Sample(sampleType, i.texc5)*w0;
	color += shaderTex.Sample(sampleType, i.texc6)*w1;
	color += shaderTex.Sample(sampleType, i.texc7)*w2;
	color += shaderTex.Sample(sampleType, i.texc8)*w3;
	color += shaderTex.Sample(sampleType, i.texc9)*w4;

	color.a = 1;

	return color;
}