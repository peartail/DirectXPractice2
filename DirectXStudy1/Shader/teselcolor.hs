cbuffer TessellationBuffer
{
	float tesselationAmount;
	float3 padding;
};

struct HullIType
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

struct ConstOType
{
	float edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
};

struct HullOType
{
	float3 pos : POSITION;
	float4 color :COLOR;
};

//////////////////////////////////////////

ConstOType CPS(InputPatch<HullIType,3> i,uint patchid : SV_PrimitiveID)
{
	ConstOType o;

	o.edges[0] = tesselationAmount;
	o.edges[1] = tesselationAmount;
	o.edges[2] = tesselationAmount;

	o.inside = tesselationAmount;

	return o;
}



////////////////////////////////////////////////////////
////////////// Hull Shader /////////////////////////////
////////////////////////////////////////////////////////
[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CPS")]

HullOType CHS(InputPatch<HullIType, 3> patch, uint pointid : SV_OutputControlPointID, uint patchid : SV_PrimitiveID)
{
	HullOType o;

	o.pos = patch[pointid].pos;

	o.color = patch[pointid].color;

	return o;
}
