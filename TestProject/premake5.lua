project "TestProject"
kind "ConsoleApp"
language "C++"
cppdialect "C++17"
staticruntime "off"
		
targetdir ("bin/")
objdir ("bin-int/" .. outputdir)

debugdir "%{wks.location}"
		
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
	"%{IncludeDir.JingleEngine}"
}

links
{
	"JingleScript",
	"JingleEngine"
}

defines
{
	"_CRT_SECURE_NO_WARNINGS",
	"FMT_HEADER_ONLY"
}

filter "system:windows"
	systemversion "latest"
	linkoptions
	{
		"/WHOLEARCHIVE:JingleScript.lib",
		"/WHOLEARCHIVE:JingleEngine.lib"
	}
	
filter "configurations:Debug"
	runtime "Debug"
	symbols "on"
	
filter "configurations:Release"
	runtime "Release"
	optimize "on"
