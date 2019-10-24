#pragma once


#ifdef MZ_PLATFORM_WINDOWS

extern Moza::Application* Moza::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Moza::CreateApplication();
	app->Run();
	delete app;
}

#endif