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
IncludeDir["glm"] = "Silver/vendor/glm"

include "Silver/vendor/GLFW"
include "Silver/vendor/Glad"
include "Silver/vendor/ImGui"

project "Silver"
	location "Silver"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Silver/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SV_PLATFORM_WINDOWS",
			"SV_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "SV_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SV_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SV_DIST"
		runtime "Release"
		optimize "on"

project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"%{IncludeDir.glm}"
	}

	links
	{
		"Silver"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SV_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "SV_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SV_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SV_DIST"
		runtime "Release"
		optimize "on"