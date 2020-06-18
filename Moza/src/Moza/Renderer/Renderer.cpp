#include "mzpch.h"
#include "Moza/Renderer/Renderer.h"
#include "Moza/Renderer/Renderer2D.h"

#include <glad/glad.h>

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
		const glm::vec4& clearColor = renderPass->GetSpecification().TargetFramebuffer->GetSpecification().ClearColor;
		RendererCommand::SetClearColor(clearColor);
	}

	void Renderer::IEndRenderPass()
	{
		MZ_CORE_ASSERT(m_ActiveRenderPass, "No active render pass! Have you called Renderer::EndRenderPass twice?");
		m_ActiveRenderPass->GetSpecification().TargetFramebuffer->Unbind();
		m_ActiveRenderPass = nullptr;
	}

	void Renderer::SubmitMeshI(const Ref<Mesh>& mesh, const glm::mat4& transform, const Ref<MaterialInstance>& overrideMaterial)
	{
		if (overrideMaterial)
		{
			overrideMaterial->Bind();
		}
		else
		{
			// Bind Mesh Material Here
		}

		// TODO: Sort this out
		mesh->m_VertexArray->Bind();

		for (Submesh& submesh : mesh->m_Submeshes)
		{
			if (mesh->m_IsAnimated)
			{
				for (size_t i = 0; i < mesh->m_BoneTransforms.size(); i++)
				{
					std::string uniformName = std::string("u_BoneTransforms[") + std::to_string(i) + std::string("]");
					mesh->m_MeshShader->SetMat4(uniformName, mesh->m_BoneTransforms[i]);
				}
			}

			glDrawElementsBaseVertex(GL_TRIANGLES, submesh.IndexCount, GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * submesh.BaseIndex), submesh.BaseVertex);
		}
	}
}