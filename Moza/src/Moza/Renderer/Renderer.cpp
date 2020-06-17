#include "mzpch.h"
#include "Moza/Renderer/Renderer.h"
#include "Moza/Renderer/Renderer2D.h"

namespace Moza
{
	Renderer* Renderer::s_Instance = new Renderer();

	void Renderer::Init()
	{
		MZ_PROFILE_FUNCTION();

		s_Instance->m_ShaderLibrary = CreateScope<ShaderLibrary>();

		RendererCommand::Init();
		Renderer2D::Init();

		Renderer::GetShaderLibrary()->Load("assets/shaders/MozaPBR_Static.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/MozaPBR_Anim.glsl");
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::WaitAndRender()
	{
	}

	void Renderer::IBeginRenderPass(const Ref<RenderPass>& renderPass)
	{
		// TODO: Convert all of this into a render command buffer
		m_ActiveRenderPass = renderPass;

		renderPass->GetSpecification().TargetFramebuffer->Bind();
	}

	void Renderer::IEndRenderPass()
	{
		MZ_CORE_ASSERT(m_ActiveRenderPass, "No active render pass! Have you called Renderer::EndRenderPass twice?");
		m_ActiveRenderPass->GetSpecification().TargetFramebuffer->Unbind();
		m_ActiveRenderPass = nullptr;
	}

	void Renderer::SubmitMeshI(const Ref<Mesh>& mesh)
	{
	}
}