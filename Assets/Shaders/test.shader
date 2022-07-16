{ShaderGraph}: {
 Nodes: [
  {ShaderNode_Output, Output}: {
   EditorPositionX: "905.000000", 
   EditorPositionY: "346.000000"
  }, 
  {ShaderNode_Sample, Sample_Roughness}: {
   {NodeConnectionArray, ConnectionsData}: [
    {NodeConnection, Output}: {
     Node: "Output", 
     Pin: "Roughness"
    }
   ], 
   EditorPositionX: "400.000000", 
   EditorPositionY: "220.000000"
  }, 
  {ShaderNode_Variable, Image_Roughness}: {
   {NodeConnectionArray, ConnectionsData}: [
    {NodeConnection, Output}: {
     Node: "Sample_Roughness", 
     Pin: "UV"
    }
   ], 
   EditorPositionX: "50.000000", 
   EditorPositionY: "220.000000", 
   Name: ""
  }, 
  {ShaderNode_Sample, Sample_Normal}: {
   {NodeConnectionArray, ConnectionsData}: [
    {NodeConnection, Output}: {
     Node: "Output", 
     Pin: "Normal"
    }
   ], 
   EditorPositionX: "400.000000", 
   EditorPositionY: "380.000000"
  }, 
  {ShaderNode_UV, Image_Metallic}: {
   {NodeConnectionArray, ConnectionsData}: [
    {NodeConnection, UV}: {
     Node: "Sample_Metallic", 
     Pin: "UV"
    }
   ], 
   EditorPositionX: "50.000000", 
   EditorPositionY: "460.000000"
  }, 
  {ShaderNode_Sample, Sample_Diffuse}: {
   {NodeConnectionArray, ConnectionsData}: [
    {NodeConnection, Output}: {
     Node: "Output", 
     Pin: "Diffuse"
    }
   ], 
   EditorPositionX: "400.000000", 
   EditorPositionY: "300.000000"
  }, 
  {ShaderNode_Variable, Image_Diffuse}: {
   {NodeConnectionArray, ConnectionsData}: [
    {NodeConnection, Output}: {
     Node: "Sample_Diffuse", 
     Pin: "UV"
    }
   ], 
   EditorPositionX: "50.000000", 
   EditorPositionY: "300.000000", 
   Name: ""
  }, 
  {ShaderNode_Variable, Image_Normal}: {
   {NodeConnectionArray, ConnectionsData}: [
    {NodeConnection, Output}: {
     Node: "Sample_Normal", 
     Pin: "UV"
    }
   ], 
   EditorPositionX: "50.000000", 
   EditorPositionY: "380.000000", 
   Name: ""
  }, 
  {ShaderNode_Sample, Sample_Metallic}: {
   {NodeConnectionArray, ConnectionsData}: [
    {NodeConnection, Output}: {
     Node: "Output", 
     Pin: "Metallic"
    }
   ], 
   EditorPositionX: "400.000000", 
   EditorPositionY: "460.000000"
  }, 
  {ShaderNode_Sample, Sample_Ambient}: {
   {NodeConnectionArray, ConnectionsData}: [
    {NodeConnection, Output}: {
     Node: "Output", 
     Pin: "Ambient"
    }
   ], 
   EditorPositionX: "400.000000", 
   EditorPositionY: "540.000000"
  }, 
  {ShaderNode_Variable, Image_Ambient}: {
   {NodeConnectionArray, ConnectionsData}: [
    {NodeConnection, Output}: {
     Node: "Sample_Ambient", 
     Pin: "UV"
    }
   ], 
   EditorPositionX: "50.000000", 
   EditorPositionY: "540.000000", 
   Name: ""
  }
 ]
}