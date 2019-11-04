#include "mzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Moza
{
	OpenGLContext::OpenGLContext(GLFWwindow * windowHandle)
		: m_WindowHandle(windowHandle)
	{
		MZ_CORE_ASSERT(m_WindowHandle, "Window Handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		//load glad
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MZ_CORE_ASSERT(status, "Failed to initialize Glad");

		MZ_CORE_INFO("OpenGL Info:");
		MZ_CORE_INFO("  Vender:   {0}", glGetString(GL_VENDOR));
		MZ_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		MZ_CORE_INFO("  Version:  {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}