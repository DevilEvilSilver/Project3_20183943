workspace "Project3"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["spdlog"] = "Silver/vendor/spdlog/include"
IncludeDir["GLFW"] = "Silver/vendor/GLFW/include"
IncludeDir["Glad"] = "Silver/vendor/Glad/include"
IncludeDir["ImGui"] = "Silver/vendor/ImGui"

include "Silver/vendor/GLFW"
include "Silver/vendor/Glad"
include "Silver/vendor/ImGui"

project "Silver"
	location "Silver"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Silver/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"SV_PLATFORM_WINDOWS",
			"SV_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Game")
		}

	filter "configurations:Debug"
		defines "SV_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "SV_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "SV_DIST"
		buildoptions "/MD"
		optimize "On"

project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Silver/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
	}

	links
	{
		"Silver"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"SV_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "SV_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "SV_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "SV_DIST"
		buildoptions "/MD"
		optimize "On"