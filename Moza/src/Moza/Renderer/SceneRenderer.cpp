#include "mzpch.h"
#include "SceneRenderer.h"

#include "Renderer.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

namespace Moza
{
	struct SceneRendererData
	{
		const Scene* ActiveScene = nullptr;
		struct SceneInfo
		{
			Camera SceneCamera;

			// Resources
			//Ref<MaterialInstance> SkyboxMaterial;
			Ref<Shader> SkyboxShader;
			Environment SceneEnvironment;
		} SceneData;

		Ref<Texture2D> BRDFLUT;
		Ref<Shader> CompositeShader;

		Ref<RenderPass> GeoPass;
		Ref<RenderPass> CompositePass;

		struct DrawCommand
		{
			Ref<Mesh> Mesh;
			Ref<MaterialInstance> Material;
			glm::mat4 Transform;
		};
		std::vector<DrawCommand> DrawList;

		// Grid
		//Ref<MaterialInstance> GridMaterial;
		Ref<Mesh> GridMesh;
		Ref<Shader> GridShader;
	};

	static SceneRendererData s_Data;

	void SceneRenderer::Init()
	{
		FramebufferSpecification geoFramebufferSpec;
		geoFramebufferSpec.Width = 1280;
		geoFramebufferSpec.Height = 720;
		geoFramebufferSpec.Format = FramebufferFormat::RGBA16F;
		geoFramebufferSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };

		RenderPassSpecification geoRenderPassSpec;
		geoRenderPassSpec.TargetFramebuffer = Moza::Framebuffer::Create(geoFramebufferSpec);
		s_Data.GeoPass = RenderPass::Create(geoRenderPassSpec);

		FramebufferSpecification compFramebufferSpec;
		compFramebufferSpec.Width = 1280;
		compFramebufferSpec.Height = 720;
		compFramebufferSpec.Format = FramebufferFormat::RGBA8;
		compFramebufferSpec.ClearColor = { 0.5f, 0.1f, 0.1f, 1.0f };

		RenderPassSpecification compRenderPassSpec;
		compRenderPassSpec.TargetFramebuffer = Moza::Framebuffer::Create(compFramebufferSpec);
		s_Data.CompositePass = RenderPass::Create(compRenderPassSpec);

		s_Data.CompositeShader = Shader::Create("assets/shaders/hdr.glsl");
		s_Data.CompositeShader->Bind();
		s_Data.CompositeShader->SetInt("u_Texture", 0);
		s_Data.CompositeShader->UnBind();

		s_Data.BRDFLUT = Texture2D::Create("assets/textures/BRDF_LUT.tga");

		// Grid
		s_Data.GridMesh = CreateRef<Mesh>("assets/models/Plane1m.obj");
		s_Data.GridShader = Shader::Create("assets/shaders/grid.glsl");
		float gridScale = 16.025f, gridSize = 0.025f;
		s_Data.GridShader->Bind();
		s_Data.GridShader->SetFloat("u_Scale", gridScale);
		s_Data.GridShader->SetFloat("u_Res", gridSize);
	}

	void SceneRenderer::SetViewportSize(uint32_t width, uint32_t height)
	{
		s_Data.GeoPass->GetSpecification().TargetFramebuffer->Resize(width, height);
		s_Data.CompositePass->GetSpecification().TargetFramebuffer->Resize(width, height);
	}

	void SceneRenderer::BeginScene(const Scene* scene)
	{
		MZ_CORE_ASSERT(!s_Data.ActiveScene, "");

		s_Data.ActiveScene = scene;

		s_Data.SceneData.SceneCamera = scene->m_Camera;
		s_Data.SceneData.SkyboxShader = scene->m_SkyboxShader;
		s_Data.SceneData.SceneEnvironment = scene->m_Environment;
	}

	void SceneRenderer::EndScene()
	{
		MZ_CORE_ASSERT(s_Data.ActiveScene, "");

		s_Data.ActiveScene = nullptr;

		FlushDrawList();
	}

	void SceneRenderer::SubmitEntity(Entity* entity)
	{
		// TODO: Culling, sorting, etc.

		auto mesh = entity->GetMesh();
		if (!mesh)
			return;

		s_Data.DrawList.push_back({ mesh, entity->GetMaterial(), entity->GetTransform() });
	}

	static Ref<Shader> equirectangularConversionShader, envFilteringShader, envIrradianceShader;

	std::pair<Ref<TextureCube>, Ref<TextureCube>> SceneRenderer::CreateEnviromentMap(const std::string& filepath)
	{
		const uint32_t cubemapSize = 2048;
		const uint32_t irradianceMapSize = 32;

		Ref<TextureCube> envUnfiltered = TextureCube::Create(TextureFormat::Float16, cubemapSize, cubemapSize);
		if (!equirectangularConversionShader)
			equirectangularConversionShader = Shader::Create("assets/shaders/EquirectangularToCubeMap.glsl");
		Ref<Texture2D> envEquirect = Texture2D::Create(filepath);
		MZ_CORE_ASSERT(envEquirect->GetFormat() == TextureFormat::Float16, "Texture is not HDR!");

		equirectangularConversionShader->Bind();
		envEquirect->Bind();

		glBindImageTexture(0, envUnfiltered->GetRendererID(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16F);
		glDispatchCompute(cubemapSize / 32, cubemapSize / 32, 6);
		glGenerateTextureMipmap(envUnfiltered->GetRendererID());

		if (!envFilteringShader)
			envFilteringShader = Shader::Create("assets/shaders/EnvironmentMipFilter.glsl");

		Ref<TextureCube> envFiltered = TextureCube::Create(TextureFormat::Float16, cubemapSize, cubemapSize);

		glCopyImageSubData(envUnfiltered->GetRendererID(), GL_TEXTURE_CUBE_MAP, 0, 0, 0, 0,
			envFiltered->GetRendererID(), GL_TEXTURE_CUBE_MAP, 0, 0, 0, 0,
			envFiltered->GetWidth(), envFiltered->GetHeight(), 6);

		envFilteringShader->Bind();
		envUnfiltered->Bind();

		const float deltaRoughness = 1.0f / glm::max((float)(envFiltered->GetMipLevelCount() - 1.0f), 1.0f);
		for (int level = 1, size = cubemapSize / 2; level < envFiltered->GetMipLevelCount(); level++, size /= 2) // <= ?
		{
			const GLuint numGroups = glm::max(1, size / 32);
			glBindImageTexture(0, envFiltered->GetRendererID(), level, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16F);
			glProgramUniform1f(envFilteringShader->GetRendererID(), 0, level * deltaRoughness);
			glDispatchCompute(numGroups, numGroups, 6);
		}

		if (!envIrradianceShader)
			envIrradianceShader = Shader::Create("assets/shaders/EnvironmentIrradiance.glsl");

		Ref<TextureCube> irradianceMap = TextureCube::Create(TextureFormat::Float16, irradianceMapSize, irradianceMapSize);
		envIrradianceShader->Bind();
		envFiltered->Bind();

		glBindImageTexture(0, irradianceMap->GetRendererID(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16F);
		glDispatchCompute(irradianceMap->GetWidth() / 32, irradianceMap->GetHeight() / 32, 6);

		return { envFiltered, irradianceMap };
	}

	Ref<Texture2D> SceneRenderer::GetFinalColorBuffer()
	{
		// return s_Data.CompositePass->GetSpecification().TargetFramebuffer;
		MZ_CORE_ASSERT(false, "Not implemented");
		return nullptr;
	}

	uint32_t SceneRenderer::GetFinalColorBufferRendererID()
	{
		return s_Data.CompositePass->GetSpecification().TargetFramebuffer->GetColorAttachmentRendererID();
	}

	void SceneRenderer::FlushDrawList()
	{
		MZ_CORE_ASSERT(!s_Data.ActiveScene, "");

		GeometryPass();
		CompositePass();

		s_Data.DrawList.clear();
		s_Data.SceneData = {};
	}

	void SceneRenderer::GeometryPass()
	{
		Renderer::BeginRenderPass(s_Data.GeoPass);

		auto viewProjection = s_Data.SceneData.SceneCamera.GetProjectionMatrix() * s_Data.SceneData.SceneCamera.GetViewMatrix();

		// Skybox
		s_Data.SceneData.SkyboxShader->Bind();
		s_Data.SceneData.SkyboxShader->SetMat4("u_InverseVP", glm::inverse(viewProjection));
		Renderer::SubmitFullscreenQuad(nullptr, false);

		// Render entities
		for (auto& dc : s_Data.DrawList)
		{
			//auto baseMaterial = dc.Mesh->GetMaterial();
			//baseMaterial->Set("u_ViewProjectionMatrix", viewProjection);
			//baseMaterial->Set("u_CameraPosition", s_Data.SceneData.SceneCamera.GetPosition());
			//
			//// Environment (TODO: don't do this per mesh)
			//baseMaterial->Set("u_EnvRadianceTex", s_Data.SceneData.SceneEnvironment.RadianceMap);
			//baseMaterial->Set("u_EnvIrradianceTex", s_Data.SceneData.SceneEnvironment.IrradianceMap);
			//baseMaterial->Set("u_BRDFLUTTexture", s_Data.BRDFLUT);

			auto baseShader = dc.Mesh->GetMeshShader();
			baseShader->Bind();
			baseShader->SetMat4("u_ViewProjectionMatrix", viewProjection);
			baseShader->SetFloat3("u_CameraPosition", s_Data.SceneData.SceneCamera.GetPosition());
			
			// Environment (TODO: don't do this per mesh)
			baseShader->SetInt("u_EnvRadianceTex", 0);
			baseShader->SetInt("u_EnvIrradianceTex", 1);
			baseShader->SetInt("u_BRDFLUTTexture", 2);
			s_Data.SceneData.SceneEnvironment.RadianceMap->Bind(0);
			s_Data.SceneData.SceneEnvironment.IrradianceMap->Bind(1);
			s_Data.BRDFLUT->Bind(2);

			auto overrideMaterial = nullptr; // dc.Material;
			Renderer::SubmitMesh(dc.Mesh, dc.Transform, overrideMaterial);
		}

		// Grid
		s_Data.GridShader->Bind();
		s_Data.GridShader->SetMat4("u_ViewProjection", viewProjection);
		s_Data.GridShader->SetMat4("u_Transform", glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(16.0f)));
		Renderer::SubmitQuad(nullptr, glm::mat4(1.0f));
		//s_Data.GridShader->SetMat4("u_Transform", glm::scale(glm::mat4(1.0f), glm::vec3(16.0f)));
		//s_Data.GridMesh->Render(0.0f, s_Data.GridShader.get());

		Renderer::EndRenderPass();
	}

	void SceneRenderer::CompositePass()
	{
		Renderer::BeginRenderPass(s_Data.CompositePass);
		s_Data.CompositeShader->Bind();
		s_Data.CompositeShader->SetFloat("u_Exposure", s_Data.SceneData.SceneCamera.GetExposure());
		s_Data.GeoPass->GetSpecification().TargetFramebuffer->BindTexture();
		Renderer::SubmitFullscreenQuad(nullptr, false);
		Renderer::EndRenderPass();
	}
}
