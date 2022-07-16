{ShaderGraph}: {
 Nodes: [
  {ShaderNode_Output, Output}: {
   EditorPositionX: "905.000000", 
   EditorPositionY: "346.000000"
  }, 
  {ShaderNode_Sample, Sample_Roughness}: {
   EditorPositionX: "400.000000", 
   EditorPositionY: "220.000000", 
   {NodeOutConnectionArray, Out}: [
    {NodeOutConnection, Output}: {
     {NodeInConnectionArray, In}: [
      {NodeInConnection, 0}: {
       Node: "Output", 
       Pin: "Roughness"
      }
     ]
    }
   ]
  }, 
  {ShaderNode_Variable, Image_Roughness}: {
   EditorPositionX: "50.000000", 
   EditorPositionY: "220.000000", 
   Name: "", 
   {NodeOutConnectionArray, Out}: [
    {NodeOutConnection, Output}: {
     {NodeInConnectionArray, In}: [
      {NodeInConnection, 0}: {
       Node: "Sample_Roughness", 
       Pin: "Image"
      }
     ]
    }
   ]
  }, 
  {ShaderNode_Sample, Sample_Normal}: {
   EditorPositionX: "400.000000", 
   EditorPositionY: "380.000000", 
   {NodeOutConnectionArray, Out}: [
    {NodeOutConnection, Output}: {
     {NodeInConnectionArray, In}: [
      {NodeInConnection, 0}: {
       Node: "Output", 
       Pin: "Normal"
      }
     ]
    }
   ]
  }, 
  {ShaderNode_UV, Image_Metallic}: {
   EditorPositionX: "50.000000", 
   EditorPositionY: "460.000000", 
   {NodeOutConnectionArray, Out}: [
    {NodeOutConnection, UV}: {
     {NodeInConnectionArray, In}: [
      {NodeInConnection, 0}: {
       Node: "Sample_Metallic", 
       Pin: "Image"
      }
     ]
    }
   ]
  }, 
  {ShaderNode_Sample, Sample_Diffuse}: {
   EditorPositionX: "400.000000", 
   EditorPositionY: "300.000000", 
   {NodeOutConnectionArray, Out}: [
    {NodeOutConnection, Output}: {
     {NodeInConnectionArray, In}: [
      {NodeInConnection, 0}: {
       Node: "Output", 
       Pin: "Diffuse"
      }
     ]
    }
   ]
  }, 
  {ShaderNode_Variable, Image_Diffuse}: {
   EditorPositionX: "50.000000", 
   EditorPositionY: "300.000000", 
   Name: "", 
   {NodeOutConnectionArray, Out}: [
    {NodeOutConnection, Output}: {
     {NodeInConnectionArray, In}: [
      {NodeInConnection, 0}: {
       Node: "Sample_Diffuse", 
       Pin: "Image"
      }
     ]
    }
   ]
  }, 
  {ShaderNode_Variable, Image_Normal}: {
   EditorPositionX: "50.000000", 
   EditorPositionY: "380.000000", 
   Name: "", 
   {NodeOutConnectionArray, Out}: [
    {NodeOutConnection, Output}: {
     {NodeInConnectionArray, In}: [
      {NodeInConnection, 0}: {
       Node: "Sample_Normal", 
       Pin: "Image"
      }
     ]
    }
   ]
  }, 
  {ShaderNode_Sample, Sample_Metallic}: {
   EditorPositionX: "400.000000", 
   EditorPositionY: "460.000000", 
   {NodeOutConnectionArray, Out}: [
    {NodeOutConnection, Output}: {
     {NodeInConnectionArray, In}: [
      {NodeInConnection, 0}: {
       Node: "Output", 
       Pin: "Metallic"
      }
     ]
    }
   ]
  }, 
  {ShaderNode_Sample, Sample_Ambient}: {
   EditorPositionX: "400.000000", 
   EditorPositionY: "540.000000", 
   {NodeOutConnectionArray, Out}: [
    {NodeOutConnection, Output}: {
     {NodeInConnectionArray, In}: [
      {NodeInConnection, 0}: {
       Node: "Output", 
       Pin: "Ambient"
      }
     ]
    }
   ]
  }, 
  {ShaderNode_Variable, Image_Ambient}: {
   EditorPositionX: "50.000000", 
   EditorPositionY: "540.000000", 
   Name: "", 
   {NodeOutConnectionArray, Out}: [
    {NodeOutConnection, Output}: {
     {NodeInConnectionArray, In}: [
      {NodeInConnection, 0}: {
       Node: "Sample_Ambient", 
       Pin: "Image"
      }
     ]
    }
   ]
  }
 ]
}