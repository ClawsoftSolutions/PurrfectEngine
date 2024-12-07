// struct VSInput {
//   [[vk::location(0)]] float2 Position : POSITION0;
//   [[vk::location(1)]] float2 UV : TEXCOORD0;
// };

// struct VSOutput {
// 	float4 Position : SV_POSITION;
//   [[vk::location(0)]] float2 UV : TEXCOORD0;
// };

// VSOutput main(VSInput input) {
// 	VSOutput output = (VSOutput)0;
// 	output.Position = float4(input.Position, 0.0f, 1.0f);
// 	output.UV       = input.UV;
// 	return output;
// }





static float2 positions[3] = {
	float2( 0.0f, -1.0f),
	float2( 1.0f,  1.0f),
	float2(-1.0f,  1.0f),
};

struct VSOutput {
	float4 Position : SV_POSITION;
};

VSOutput main(uint index : SV_VertexID) {
	VSOutput output = (VSOutput)0;
	output.Position = float4(positions[index], 0.0f, 1.0f);
	return output;
}