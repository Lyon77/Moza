#pragma once

#include "Moza/Core/Core.h"

#ifdef MZ_PLATFORM_WINDOWS

extern Moza::Application* Moza::CreateApplication();

int main(int argc, char** argv)
{
	Moza::Log::Init();

	MZ_PROFILE_BEGIN_SESSION("Startup", "MozaProfile-Startup.json");
	auto app = Moza::CreateApplication();
	MZ_PROFILE_END_SESSION();

	MZ_PROFILE_BEGIN_SESSION("Startup", "MozaProfile-Runtime.json");
	app->Run();
	MZ_PROFILE_END_SESSION();

	MZ_PROFILE_BEGIN_SESSION("Startup", "MozaProfile-Shutdown.json");
	delete app;
	MZ_PROFILE_END_SESSION();
}

#endif