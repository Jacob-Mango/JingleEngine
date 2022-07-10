project "TestProject"
kind "ConsoleApp"
language "C++"
cppdialect "C++20"
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
	"%{IncludeDir.imgui}",
	"%{IncludeDir.imnodes}",
	"%{IncludeDir.stb}",
	"%{IncludeDir.Assimp}",
	"%{IncludeDir.glm}",
	"%{IncludeDir.glew}",
	"%{IncludeDir.uuid_v4}",
	"%{IncludeDir.JingleEngine}"
}

links
{
	"JingleScript",
	"ImGui",
	"JingleEngine",
	"%{Library.Glew}",
	"opengl32.lib"
}

defines
{
	"_CRT_SECURE_NO_WARNINGS",
	"FMT_HEADER_ONLY",
	"GLEW_STATIC"
}

filter "system:windows"
	systemversion "latest"
	linkoptions
	{
		"/WHOLEARCHIVE:JingleScript.lib",
		"/WHOLEARCHIVE:JingleEngine.lib"
	}

	defines
	{
		"WINDOWS"
	}
	
filter "configurations:Debug"
	runtime "Debug"
	symbols "on"

	links
	{
		"%{Library.Assimp_Debug}"
	}

	postbuildcommands 
	{
		'{COPY} "%{Binaries.Assimp_Debug}" "%{cfg.targetdir}"'
	}
	
filter "configurations:Release"
	runtime "Release"
	optimize "on"

	links
	{
		"%{Library.Assimp_Release}"
	}

	postbuildcommands 
	{
		'{COPY} "%{Binaries.Assimp_Release}" "%{cfg.targetdir}"'
	}
