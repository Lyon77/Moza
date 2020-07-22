#include "mzpch.h"
#include "Moza/Renderer/Renderer.h"
#include "Moza/Renderer/Renderer2D.h"

#include "Moza/Renderer/SceneRenderer.h"

#include <glad/glad.h>

namespace Moza
{
	struct RendererData
	{
		Ref<RenderPass> m_ActiveRenderPass;
		//RenderCommandQueue m_CommandQueue;
		Scope<ShaderLibrary> m_ShaderLibrary;
		Ref<VertexArray> m_FullscreenQuadVertexArray;
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		MZ_PROFILE_FUNCTION();

		s_Data.m_ShaderLibrary = CreateScope<ShaderLibrary>();

		RendererCommand::Init();
		Renderer2D::Init();

		Renderer::GetShaderLibrary()->Load("assets/shaders/MozaPBR_Static.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/MozaPBR_Anim.glsl");

		SceneRenderer::Init();

		// Create Fullscreen Quad
		float x = -1;
		float y = -1;
		float width = 2, height = 2;
		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec2 TexCoord;
		};

		QuadVertex* data = new QuadVertex[4];

		data[0].Position = glm::vec3(x, y, 0);
		data[0].TexCoord = glm::vec2(0, 0);

		data[1].Position = glm::vec3(x + width, y, 0);
		data[1].TexCoord = glm::vec2(1, 0);

		data[2].Position = glm::vec3(x + width, y + height, 0);
		data[2].TexCoord = glm::vec2(1, 1);

		data[3].Position = glm::vec3(x, y + height, 0);
		data[3].TexCoord = glm::vec2(0, 1);

		s_Data.m_FullscreenQuadVertexArray = VertexArray::Create();
		auto quadVB = VertexBuffer::Create((float*)data, 4 * sizeof(QuadVertex));
		quadVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0, };
		auto quadIB = IndexBuffer::Create(indices, 6);

		s_Data.m_FullscreenQuadVertexArray->AddVertexBuffer(quadVB);
		s_Data.m_FullscreenQuadVertexArray->SetIndexBuffer(quadIB);

	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	const Scope<ShaderLibrary>& Renderer::GetShaderLibrary()
	{
		return s_Data.m_ShaderLibrary;
	}

	void Renderer::WaitAndRender()
	{
		// s_Data.m_CommandQueue.Execute();
	}

	void Renderer::BeginRenderPass(const Ref<RenderPass>& renderPass)
	{
		// TODO: Convert all of this into a render command buffer
		s_Data.m_ActiveRenderPass = renderPass;

		renderPass->GetSpecification().TargetFramebuffer->Bind();
		const glm::vec4& clearColor = renderPass->GetSpecification().TargetFramebuffer->GetSpecification().ClearColor;
		RendererCommand::SetClearColor(clearColor);
	}

	void Renderer::EndRenderPass()
	{
		MZ_CORE_ASSERT(s_Data.m_ActiveRenderPass, "No active render pass! Have you called Renderer::EndRenderPass twice?");
		s_Data.m_ActiveRenderPass->GetSpecification().TargetFramebuffer->Unbind();
		s_Data.m_ActiveRenderPass = nullptr;
	}

	void Renderer::SubmitQuad(const Ref<MaterialInstance>& material, const glm::mat4& transform)
	{
		bool depthTest = false;
		if (material)
		{
			material->Bind();
			depthTest = material->GetFlag(MaterialFlag::DepthTest);

			auto shader = material->GetShader();
			shader->SetMat4("u_Transform", transform);
		}

		s_Data.m_FullscreenQuadVertexArray->Bind();
		RendererCommand::DrawIndexed(6, depthTest);
	}

	void Renderer::SubmitFullscreenQuad(const Ref<MaterialInstance>& material, bool depthTest)
	{
		if (material)
		{
			material->Bind();
			depthTest = material->GetFlag(MaterialFlag::DepthTest);
		}

		s_Data.m_FullscreenQuadVertexArray->Bind();
		RendererCommand::DrawIndexed(6, depthTest);
	}

	void Renderer::SubmitMesh(const Ref<Mesh>& mesh, const glm::mat4& transform, const Ref<MaterialInstance>& overrideMaterial)
	{
		// auto material = overrideMaterial ? overrideMaterial : mesh->GetMaterialInstance();
		// auto shader = material->GetShader();

		// TODO: Sort this out
		mesh->m_VertexArray->Bind();

		auto& materials = mesh->GetMaterials();
		for (Submesh& submesh : mesh->m_Submeshes)
		{
			// Material
			auto material = materials[submesh.MaterialIndex];
			auto shader = material->GetShader();
			material->Bind();

			if (mesh->m_IsAnimated)
			{
				for (size_t i = 0; i < mesh->m_BoneTransforms.size(); i++)
				{
					std::string uniformName = std::string("u_BoneTransforms[") + std::to_string(i) + std::string("]");
					mesh->m_MeshShader->SetMat4(uniformName, mesh->m_BoneTransforms[i]);
				}
			}
			shader->SetMat4("u_Transform", transform * submesh.Transform);

			if (material->GetFlag(MaterialFlag::DepthTest))
				glEnable(GL_DEPTH_TEST);
			else
				glDisable(GL_DEPTH_TEST);

			glDrawElementsBaseVertex(GL_TRIANGLES, submesh.IndexCount, GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * submesh.BaseIndex), submesh.BaseVertex);
		}
	}
}