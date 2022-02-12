workspace "Project3"
	architecture "x64"
	startproject "Game"
	
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
IncludeDir["ImGuizmo"] = "Silver/vendor/ImGuizmo"
IncludeDir["glm"] = "Silver/vendor/glm"
IncludeDir["entt"] = "Silver/vendor/entt/include"
IncludeDir["stb_image"] = "Silver/vendor/stb_image"
IncludeDir["tinyxml2"] = "Silver/vendor/tinyxml2"
IncludeDir["yaml_cpp"] = "Silver/vendor/yaml-cpp/include"

include "Silver/vendor/GLFW"
include "Silver/vendor/Glad"
include "Silver/vendor/ImGui"
include "Silver/vendor/yaml-cpp"

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
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"YAML_CPP_STATIC_DEFINE"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.tinyxml2}",
		"%{IncludeDir.yaml_cpp}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib",
	}

	filter "files:ImGuizmo.cpp"
		flags { "NoPCH" }

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
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.tinyxml2}"
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

project "Editor"
	location "Editor"
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
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.tinyxml2}"
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