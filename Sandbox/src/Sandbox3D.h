#pragma once

#include "Moza.h"

class Sandbox3D : public Moza::Layer
{
public:
	Sandbox3D();
	virtual ~Sandbox3D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Moza::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Moza::Event& e) override;

private:
	Moza::Ref<Moza::Shader> m_DynamicPBRShader;
	Moza::Ref<Moza::Shader> m_StaticPBRShader;
	Moza::Ref<Moza::Shader> m_QuadShader;
	Moza::Ref<Moza::Shader> m_HDRShader;
	Moza::Ref<Moza::Shader> m_GridShader;
	Moza::Ref<Moza::Mesh> m_Mesh;
	Moza::Ref<Moza::Mesh> m_SphereMesh, m_PlaneMesh;
	Moza::Ref<Moza::Texture2D> m_BRDFLUT;

	Moza::Ref<Moza::Texture2D> m_CheckerboardTex;

	Moza::Ref<Moza::MaterialInstance> m_MeshMaterial;
	Moza::Ref<Moza::MaterialInstance> m_GridMaterial;
	std::vector<Moza::Ref<Moza::MaterialInstance>> m_MetalSphereMaterialInstances;
	std::vector<Moza::Ref<Moza::MaterialInstance>> m_DielectricSphereMaterialInstances;

	float m_GridScale = 16.025f, m_GridSize = 0.025f;
	float m_MeshScale = 1.0f;

	struct AlbedoInput
	{
		glm::vec3 Color = { 0.972f, 0.96f, 0.915f }; // Silver, from https://docs.unrealengine.com/en-us/Engine/Rendering/Materials/PhysicallyBased
		Moza::Ref<Moza::Texture2D> TextureMap;
		bool SRGB = true;
		bool UseTexture = false;
	};
	AlbedoInput m_AlbedoInput;

	struct NormalInput
	{
		Moza::Ref<Moza::Texture2D> TextureMap;
		bool UseTexture = false;
	};
	NormalInput m_NormalInput;

	struct MetalnessInput
	{
		float Value = 1.0f;
		Moza::Ref<Moza::Texture2D> TextureMap;
		bool UseTexture = false;
	};
	MetalnessInput m_MetalnessInput;

	struct RoughnessInput
	{
		float Value = 0.5f;
		Moza::Ref<Moza::Texture2D> TextureMap;
		bool UseTexture = false;
	};
	RoughnessInput m_RoughnessInput;

	Moza::Ref<Moza::Framebuffer> m_Framebuffer, m_FinalPresentBuffer;
	Moza::Ref<Moza::VertexArray> m_VertexArray;
	Moza::Ref<Moza::VertexBuffer> m_VertexBuffer;
	Moza::Ref<Moza::IndexBuffer> m_IndexBuffer;
	Moza::Ref<Moza::TextureCube> m_EnvironmentCubeMap, m_EnvironmentIrradiance;

	Moza::Camera m_Camera;

	float m_ClearColor[4];

	struct Light
	{
		glm::vec3 Direction;
		glm::vec3 Radiance;
	};
	Light m_Light;
	float m_LightMultiplier = 0.3f;

	// PBR params
	float m_Exposure = 1.0f;

	bool m_RadiancePrefilter = false;

	float m_EnvMapRotation = 0.0f;

	enum class Scene : uint32_t
	{
		Spheres = 0, Model = 1
	};
	Scene m_Scene;
};