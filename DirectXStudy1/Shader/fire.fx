cbuffer MatrixBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer NoiseBuffer : register(b1)
{
	float frameTime;
	float3 scrollSpeeds;
	float3 scales;
	float padding;
};

cbuffer DistortionBuffer : register(b2)
{
	float2 dist1;
	float2 dist2;
	float2 dist3;
	float distScale;
	float distBias;
};

SamplerState sampleType;
SamplerState sampleType2;

Texture2D ftex : register(t0);
Texture2D ntex : register(t1);
Texture2D atex : register(t2);


struct VInputT
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct PInputT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float2 tex1 : TEXCOORD1;
	float2 tex2 : TEXCOORD2;
	float2 tex3 : TEXCOORD3;
};

PInputT VS(VInputT i)
{
	PInputT o;

	i.pos.w = 1.0f;

	o.pos = mul(i.pos, world);
	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);

	o.tex = i.tex;

	o.tex1 = (i.tex * scales.x);
	o.tex1.y = o.tex1.y + (frameTime * scrollSpeeds.x);

	o.tex2 = (i.tex * scales.y);
	o.tex2.y = o.tex2.y + (frameTime * scrollSpeeds.y);

	o.tex3 = (i.tex * scales.z);
	o.tex3.y = o.tex3.y + (frameTime * scrollSpeeds.z);

	return o;
}

float4 PS(PInputT i) : SV_TARGET
{
	float4 n1, n2, n3;
	float4 finaln;
	float perturb;
	float2 ncoord;
	float4 fcolor;
	float4 acolor;

	n1 = ntex.Sample(sampleType, i.tex1);
	n2 = ntex.Sample(sampleType, i.tex2);
	n3 = ntex.Sample(sampleType, i.tex3);

	n1 = (n1 - 0.5f) * 2.0f;
	n2 = (n2 - 0.5f) * 2.0f;
	n3 = (n3 - 0.5f) *2.0f;

	n1.xy = n1.xy*dist1.xy;
	n2.xy = n2.xy*dist2.xy;
	n3.xy = n3.xy*dist3.xy;

	finaln = n1 + n2 + n3;

	perturb = ((1.f - i.tex.y)*distScale) + distBias;

	ncoord.xy = (finaln.xy * perturb) + i.tex.xy;

	fcolor = ftex.Sample(sampleType2, ncoord.xy);

	acolor = atex.Sample(sampleType2, ncoord.xy);

	fcolor.a = acolor;


	return fcolor;
}