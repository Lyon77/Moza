#pragma once

#include "Moza/Core/Core.h"

#ifdef MZ_PLATFORM_WINDOWS

extern Moza::Application* Moza::CreateApplication();

int main(int argc, char** argv)
{
	Moza::Log::Init();
	MZ_CORE_WARN("Initalized Log!");
	int a = 5;
	MZ_INFO("Hello! Var={0}", a);

	auto app = Moza::CreateApplication();
	app->Run();
	delete app;
}

#endif