#include "mzpch.h"
#include "Moza/Scene/Scene.h"

#include "Moza/Renderer/SceneRenderer.h"

namespace Moza
{
	Environment Environment::Load(const std::string& filepath)
	{
		auto [radiance, irradiance] = SceneRenderer::CreateEnviromentMap(filepath);
		return { radiance, irradiance };
	}

	Scene::Scene(const std::string& debugName)
		: m_DebugName(debugName)
	{
		Init();
	}

	Scene::~Scene()
	{
		for (Entity* entity : m_Entities)
			delete entity;
	}

	void Scene::Init()
	{
		m_SkyboxShader = Shader::Create("assets/shaders/skybox.glsl");
		m_SkyboxShader->Bind();
		m_SkyboxShader->SetInt("u_Texture", 0);
		//m_SkyboxMaterial = MaterialInstance::Create(Material::Create(m_SkyboxShader));
	}

	void Scene::OnUpdate(Timestep ts)
	{
		m_Camera.Update(ts);

		m_SkyboxShader->Bind();
		m_SkyboxShader->SetFloat("u_TextureLod", m_SkyboxLod);

		// Update all entities
		for (auto entity : m_Entities)
		{
			auto mesh = entity->GetMesh();
			if (mesh)
				mesh->OnUpdate(ts);
		}

		SceneRenderer::BeginScene(this);

		// Render entities
		for (auto entity : m_Entities)
		{
			// TODO: Should we render (logically)
			SceneRenderer::SubmitEntity(entity);
		}

		SceneRenderer::EndScene();
	}

	void Scene::SetCamera(const Camera& camera)
	{
		m_Camera = camera;
	}

	void Scene::SetEnvironment(const Environment& environment)
	{
		m_Environment = environment;
		SetSkybox(environment.RadianceMap);
	}

	void Scene::SetEnvironment(const Ref<TextureCube>& radiance, const Ref<TextureCube>& irradiance)
	{
		m_Environment.RadianceMap = radiance;
		m_Environment.IrradianceMap = irradiance;
		SetSkybox(radiance);
	}

	void Scene::SetSkybox(const Ref<TextureCube>& skybox)
	{
		m_SkyboxTexture = skybox;
		m_SkyboxShader->Bind();
		m_SkyboxTexture->Bind(0);
	}

	void Scene::AddEntity(Entity* entity)
	{
		m_Entities.push_back(entity);
	}

	Entity* Scene::CreateEntity()
	{
		Entity* entity = new Entity();
		AddEntity(entity);
		return entity;
	}
}