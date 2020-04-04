workspace "Ancora"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	platforms
	{
		"linux",
		"windows",
		"macosx"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Ancora/vendor/GLFW/include"
IncludeDir["Glad"] = "Ancora/vendor/Glad/include"
IncludeDir["ImGui"] = "Ancora/vendor/imgui"
IncludeDir["glm"] = "Ancora/vendor/glm"
IncludeDir["stb_image"] = "Ancora/vendor/stb_image"

include "Ancora/vendor/GLFW"
include "Ancora/vendor/Glad"
include "Ancora/vendor/imgui"

project "Ancora"
	location "Ancora"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	warnings "Default"
	-- warnings can take the values "Off", "Default", "Extra"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "aepch.h"
	pchsource "Ancora/src/aepch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	excludes
	{
		"%{prj.name}/src/Platform/**.h",
		"%{prj.name}/src/Platform/**.cpp"
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
		"ImGui"
	}

	filter "system:linux or macosx"
		kind "SharedLib"

		defines
		{
			"AE_DYNAMIC_LINK"
		}

		links
		{
			"GL",
			"dl",
			"pthread"
		}

		files
		{
			"%{prj.name}/src/Platform/Linux/**.h",
			"%{prj.name}/src/Platform/Linux/**.cpp",
			"%{prj.name}/src/Platform/OpenGL/**.h",
			"%{prj.name}/src/Platform/OpenGL/**.cpp"
		}

		defines
		{
			"AE_PLATFORM_LINUX",
			"AE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "system:windows"
		systemversion "latest"
		kind "StaticLib"

		links
		{
			"opengl32.lib"
		}

		files
		{
			"%{prj.name}/src/Platform/Windows/**.h",
			"%{prj.name}/src/Platform/Windows/**.cpp",
			"%{prj.name}/src/Platform/OpenGL/**.h",
			"%{prj.name}/src/Platform/OpenGL/**.cpp"
		}

		defines
		{
			"AE_PLATFORM_WINDOWS",
			"AE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines
		{
			"AE_DEBUG",
			"AE_ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "AE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "AE_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
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
		"Ancora/vendor/spdlog/include",
		"Ancora/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"Ancora"
	}

	filter "system:linux or macosx"

		defines
		{
			"AE_PLATFORM_LINUX",
			"AE_BUILD_DLL"
		}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"AE_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines
		{
			"AE_DEBUG",
			"AE_ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "AE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "AE_DIST"
		runtime "Release"
		optimize "on"
