cbuffer MatrixBuffer : register (b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer LightBuffer : register (b1)
{
	float4 diffuse;
	float3 lightdir;
	float padding;
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
};

Texture2D g_tex;
SamplerState g_sample;

PInput VS(VInput i)
{
	PInput o;

	i.pos.w = 1.0f;

	o.pos = mul(i.pos, world);
	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);

	o.tex = i.tex;

	o.normal = mul(i.normal, (float3x3)world);

	o.normal = normalize(o.normal);

	return o;
}

float4 PS(PInput i) : SV_TARGET
{
	float4 texColor;
	float3 ld;
	float lightIntensity;
	float4 color;

	texColor = g_tex.Sample(g_sample, i.tex);

	ld = -lightdir;

	lightIntensity = saturate(dot(i.normal, ld));

	color = saturate(diffuse * lightIntensity);

	color = color * texColor;

	return color;
}