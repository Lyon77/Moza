#pragma once

#include "Moza/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Moza
{
	class MOZA_API OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}