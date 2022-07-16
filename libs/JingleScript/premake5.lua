project "JingleScript"
kind "StaticLib"
language "C++"
cppdialect "C++20"
staticruntime "off"

targetdir ("bin/")
objdir ("bin-int/" .. outputdir)

flags { "NoPCH" }

files
{
	"%{IncludeDir.JingleScript}/**.h", 
	"%{IncludeDir.JingleScript}/**.c", 
	"%{IncludeDir.JingleScript}/**.hpp", 
	"%{IncludeDir.JingleScript}/**.cpp" 
}

includedirs
{
	"%{IncludeDir.JingleScript}",
	"%{IncludeDir.fmt}",
}

links 
{
}

defines
{
	"_CRT_SECURE_NO_WARNINGS",
	"FMT_HEADER_ONLY"
}

filter "system:windows"
	systemversion "latest"
	
filter "configurations:DebugProject"
	runtime "Debug"
	symbols "on"
	
filter "configurations:DebugEditor"
	runtime "Debug"
	symbols "on"
	
filter "configurations:DevelopmentProject"
	runtime "Release"
	optimize "on"
	
filter "configurations:DevelopmentEditor"
	runtime "Release"
	optimize "on"
