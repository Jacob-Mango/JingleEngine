project "ImGui"
kind "StaticLib"
language "C++"
cppdialect "C++17"
staticruntime "off"

targetdir ("bin/")
objdir ("bin-int/" .. outputdir)

flags { "NoPCH" }

files
{
	"imconfig.h",
	"imgui_demo.cpp",
	"imgui_draw.cpp",
	"imgui_internal.h",
	"imgui_tables.cpp",
	"imgui_widgets.cpp",
	"imgui.cpp",
	"imgui.h",
	"imstb_rectpack.h",
	"imstb_textedit.h",
	"imstb_truetype.h",
	"backends/imgui_impl_opengl3.cpp",
	"backends/imgui_impl_opengl3.h",
	"backends/imgui_impl_opengl3_loader.h",
	"backends/imgui_impl_win32.cpp",
	"backends/imgui_impl_win32.h",
}

includedirs
{
}

defines
{
	"_CRT_SECURE_NO_WARNINGS",
	"FMT_HEADER_ONLY"
}

filter "system:windows"
	systemversion "latest"
	
filter "configurations:Debug"
	runtime "Debug"
	symbols "on"
	
filter "configurations:Release"
	runtime "Release"
	optimize "on"