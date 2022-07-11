@EditorAttribute("Shader Editor")
class ShaderEditor extends Editor
{
}

class ShaderGraph extends Graph
{

}

class ShaderNode extends Node
{

}

class ShaderNode_Variable extends ShaderNode
{

}

class ShaderNode_UV extends ShaderNode
{
	@OutPin
	vec2 UV;
}

class ShaderNode_Texture extends ShaderNode
{
	@InPin
	vec2 UV;

	@OutPin
	vec4 Output;
}

class ShaderNode_TextureVariable extends ShaderNode_Variable
{
	@InPin
	vec2 UV;

	@OutPin
	vec4 Output;
}

class ShaderNode_Color extends ShaderNode
{
	@OutPin
	vec4 Color;
}

class ShaderNode_ColorVariable extends ShaderNode_Variable
{
	@OutPin
	vec4 Color;
}

class ShaderNode_Output extends ShaderNode
{
	@InPin
	vec4 Diffuse;

	@InPin
	vec4 Normal;

	@InPin
	vec4 Metallic;

	@InPin
	vec4 Roughness;

	@InPin
	vec4 Ambient;

}
