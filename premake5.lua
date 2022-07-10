workspace "JingleEngine"
architecture "x86_64"
startproject "TestProject"
configurations
{
	"Debug",
	"Release"
}
flags
{
	"MultiProcessorCompile"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["fmt"] = "%{wks.location}/../JingleScript/libs/fmt-8.0.0/include"
IncludeDir["JingleScript"] = "%{wks.location}/../JingleScript/JingleScript/src"
IncludeDir["JingleEngine"] = "%{wks.location}/JingleEngine/src"
IncludeDir["stb"] = "%{wks.location}/libs/stb"
IncludeDir["Assimp"] = "%{wks.location}/libs/assimp/include"
IncludeDir["glm"] = "%{wks.location}/libs/glm"
IncludeDir["glew"] = "%{wks.location}/libs/glew/include"
IncludeDir["imgui"] = "%{wks.location}/libs/imgui"
IncludeDir["imnodes"] = "%{wks.location}/libs/imnodes"
IncludeDir["uuid_v4"] = "%{wks.location}/libs/uuid_v4"

Library = {}
Library["Assimp_Debug"] = "%{wks.location}/libs/assimp/lib/Debug/assimp-vc142-mtd.lib"
Library["Assimp_Release"] = "%{wks.location}/libs/assimp/lib/Release/assimp-vc142-mt.lib"
Library["Glew"] = "%{wks.location}/libs/glew/lib/Release/x64/glew32s.lib"

Binaries = {}
Binaries["Assimp_Debug"] = "%{wks.location}/libs/assimp/bin/Debug/assimp-vc142-mtd.dll"
Binaries["Assimp_Release"] = "%{wks.location}/libs/assimp/bin/Release/assimp-vc142-mt.dll"

group "Dependencies"
include "libs/imgui"

group "Core"
include "JingleEngine"
include "../JingleScript/JingleScript"

group ""
include "TestProject"
