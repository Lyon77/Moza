#include "mzpch.h"
#include "Moza/Renderer/Renderer.h"
#include "Moza/Renderer/Renderer2D.h"

namespace Moza
{
	Renderer* Renderer::s_Instance = new Renderer();
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

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

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RendererCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}
}