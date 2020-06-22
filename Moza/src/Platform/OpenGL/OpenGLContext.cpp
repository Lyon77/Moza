#include "mzpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <gl/GL.h>

namespace Moza
{
	OpenGLContext::OpenGLContext(GLFWwindow * windowHandle)
		: m_WindowHandle(windowHandle)
	{
		MZ_CORE_ASSERT(m_WindowHandle, "Window Handle is null!")
	}

	void OpenGLContext::Init()
	{
		MZ_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		glfwMaximizeWindow(m_WindowHandle);
		//load glad
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MZ_CORE_ASSERT(status, "Failed to initialize Glad");

		MZ_CORE_INFO("OpenGL Info:");
		MZ_CORE_INFO("  Vender:   {0}", glGetString(GL_VENDOR));
		MZ_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		MZ_CORE_INFO("  Version:  {0}", glGetString(GL_VERSION));

	#ifdef MZ_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		MZ_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Moza requires at least OpenGL version 4.5!");
	#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		MZ_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}