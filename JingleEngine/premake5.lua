project "JingleEngine"
kind "StaticLib"
language "C++"
cppdialect "C++17"
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
	"%{IncludeDir.stb}",
	"%{IncludeDir.Assimp}",
	"%{IncludeDir.glm}",
	"%{IncludeDir.glew}"
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
	
filter "configurations:Debug"
	runtime "Debug"
	symbols "on"
	
filter "configurations:Release"
	runtime "Release"
	optimize "on"