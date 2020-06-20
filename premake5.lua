workspace "Moza"
	architecture "x86_64"
	startproject "Mozaball"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to the root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Moza/vendor/GLFW/include"
IncludeDir["Glad"] = "Moza/vendor/Glad/include"
IncludeDir["ImGui"] = "Moza/vendor/imgui"
IncludeDir["glm"] = "Moza/vendor/glm"
IncludeDir["stb_image"] = "Moza/vendor/stb_image"

group "Dependencies"
	include "Moza/vendor/GLFW"
	include "Moza/vendor/Glad"
	include "Moza/vendor/imgui"
group ""

project "Moza"
	location "Moza"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "mzpch.h"
	pchsource "Moza/src/mzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
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
		}

	filter "configurations:Debug"
		defines "MZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "MZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "MZ_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "SandBox"
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
		"Moza/vendor/spdlog/include",
		"Moza/src",
		"Moza/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Moza"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "MZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "MZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "MZ_DIST"
		runtime "Release"
		optimize "on"

project "Mozaball"
	location "Mozaball"
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
		"Moza/vendor/spdlog/include",
		"Moza/src",
		"Moza/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Moza"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "MZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "MZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "MZ_DIST"
		runtime "Release"
		optimize "on"