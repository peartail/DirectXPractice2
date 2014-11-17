#define NUM_LIGHTS 4

cbuffer MatrixBuffer : register (b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer LightPositionBuffer : register(b1)
{
	float4 lightPos[NUM_LIGHTS];
};

cbuffer LightColorBuffer : register(b2)
{
	float4 difColor[NUM_LIGHTS];
};

struct VInput
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PInput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 lpos1 : TEXCOORD1;
	float3 lpos2 : TEXCOORD2;
	float3 lpos3 : TEXCOORD3;
	float3 lpos4 : TEXCOORD4;
};

Texture2D _tex;
SamplerState _sample;

PInput VS(VInput i)
{
	PInput o;
	float4 worldpos;

	i.pos.w = 1.f;

	o.pos = mul(i.pos, world);
	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);

	o.tex = i.tex;

	o.normal = mul(i.normal, (float3x3)world);

	o.normal = normalize(o.normal);

	worldpos = mul(i.pos, world);

	o.lpos1.xyz = lightPos[0].xyz - worldpos.xyz;
	o.lpos2.xyz = lightPos[1].xyz - worldpos.xyz;
	o.lpos3.xyz = lightPos[2].xyz - worldpos.xyz;
	o.lpos4.xyz = lightPos[3].xyz - worldpos.xyz;

	o.lpos1 = normalize(o.lpos1);
	o.lpos2 = normalize(o.lpos2);
	o.lpos3 = normalize(o.lpos3);
	o.lpos4 = normalize(o.lpos4);
	

	return o;
}

float4 PS(PInput i) : SV_TARGET
{
	float4 color;

	float4 texcolor;
	float lintens1, lintens2, lintens3, lintens4;
	float4 color1, color2, color3, color4;

	lintens1 = saturate(dot(i.normal, i.lpos1));
	lintens2 = saturate(dot(i.normal, i.lpos2));
	lintens3 = saturate(dot(i.normal, i.lpos3));
	lintens4 = saturate(dot(i.normal, i.lpos4));

	color1 = difColor[0] * lintens1;
	color2 = difColor[1] * lintens2;
	color3 = difColor[2] * lintens3;
	color4 = difColor[3] * lintens4;

	texcolor = _tex.Sample(_sample, i.tex);

	color = saturate(color1 + color2 + color3 + color4)* texcolor;

	color = float4(1, 1, 1, 1);

	return color;
}

