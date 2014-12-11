cbuffer MatrixBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
};


struct VInputT
{
	float4 pos : POSITION;
};

struct PInputT
{
	float4 pos : SV_POSITION;
	float4 depthPos : TEXTURE0;
};

PInputT VS(VInputT i)
{
	PInputT o;

	i.pos.w = 1.0f;

	o.pos = mul(i.pos, world);
	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);

	o.depthPos = o.pos;

	return o;
}

float4 PS(PInputT i) : SV_TARGET
{
	float depthV;
	float4 color;

	depthV = i.depthPos.z / i.depthPos.w;

	color = float4(depthV, depthV, depthV, 1);
	/*
	if (depthV < 0.9f)
	{
		color = float4(1, 0, 0, 1);
	}

	if (depthV > 0.9f)
	{
		color = float4(0, 1, 0, 1);
	}

	if (depthV > 0.925f)
	{
		color = float4(0, 0, 1, 1);
	}
	*/

	return color;
}