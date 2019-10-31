workspace "Moza"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to the root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Moza/vendor/GLFW/include"
IncludeDir["Glad"] = "Moza/vendor/Glad/include"
IncludeDir["ImGui"] = "Moza/vendor/imgui"

include "Moza/vendor/GLFW"
include "Moza/vendor/Glad"
include "Moza/vendor/imgui"
group "Dependencies"
	include "Moza/vendor/GLFW"
	include "Moza/vendor/Glad"
	include "Moza/vendor/imgui"
	
group ""

project "Moza"
	location "Moza"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "mzpch.h"
	pchsource "Moza/src/mzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
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
		systemversion "latest"

		defines
		{
			"MZ_PLATFORM_WINDOWS",
			"MZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			{"{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\""}
		}

	filter "configurations:Debug"
		defines "MZ_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "MZ_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "MZ_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "SandBox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"Moza/vendor/spdlog/include",
		"Moza/src"
	}

	links
	{
		"Moza"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"MZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "MZ_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "MZ_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "MZ_DIST"
		runtime "Release"
		optimize "On"