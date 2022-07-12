{ShaderGraph}: {
 Nodes: [
  {ShaderNode_Output, Output}: {

  }, 
  {ShaderNode_Texture, Roughness}: {
   {NodeConnectionArray, ConnectionsData}: [
    {NodeConnection, Output}: {
     Node: "Output", 
     Pin: "Roughness"
    }
   ]
  }, 
  {ShaderNode_Texture, Diffuse}: {
   {NodeConnectionArray, ConnectionsData}: [
    {NodeConnection, Output}: {
     Node: "Output", 
     Pin: "Diffuse"
    }
   ]
  }
 ]
}