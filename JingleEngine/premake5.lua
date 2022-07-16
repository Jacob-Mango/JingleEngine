project "JingleEngine"
kind "StaticLib"
language "C++"
cppdialect "C++20"
staticruntime "off"

targetdir ("bin/")
objdir ("bin-int/" .. outputdir)

flags { "NoPCH" }

files
{
	"src/**.h", 
	"src/**.c", 
	"src/**.hpp", 
	"src/**.cpp" 
}

includedirs
{
	"src",
	"%{IncludeDir.fmt}",
	"%{IncludeDir.JingleScript}",
	"%{IncludeDir.imgui}",
	"%{IncludeDir.imnodes}",
	"%{IncludeDir.stb}",
	"%{IncludeDir.Assimp}",
	"%{IncludeDir.glm}",
	"%{IncludeDir.glew}",
	"%{IncludeDir.uuid_v4}"
}

defines
{
	"_CRT_SECURE_NO_WARNINGS",
	"FMT_HEADER_ONLY",
	"GLEW_STATIC"
}

filter "system:windows"
	systemversion "latest"

	defines
	{
		"WINDOWS"
	}
	
filter "configurations:DebugProject"
	runtime "Debug"
	symbols "on"

	removefiles 
	{
		"src/Editor/**"
	}
	
filter "configurations:DebugEditor"
	runtime "Debug"
	symbols "on"

	defines
	{
		"JE_EDITOR"
	}
	
filter "configurations:DevelopmentProject"
	runtime "Release"
	optimize "on"
	
	removefiles 
	{
		"src/Editor/**"
	}
	
filter "configurations:DevelopmentEditor"
	runtime "Release"
	optimize "on"

	defines
	{
		"JE_EDITOR"
	}
