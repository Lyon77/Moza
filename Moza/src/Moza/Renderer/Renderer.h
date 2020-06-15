#pragma once

#include "Moza/Renderer/RendererCommand.h"
#include "Moza/Renderer/OrthographicCamera.h"
#include "Moza/Renderer/Shader.h"

namespace Moza
{
	class ShaderLibrary;

	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Init();
		static void Shutdown();

		static const Scope<ShaderLibrary>& GetShaderLibrary() { return Get().m_ShaderLibrary; }

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		inline static Renderer& Get() { return *s_Instance; }
	private:
		static Renderer* s_Instance;

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;

		Scope<ShaderLibrary> m_ShaderLibrary;
	};
}