#pragma once

#include "Moza/Scene/Scene.h"

#include "Moza/Renderer/RenderPass.h"

namespace Moza
{
	class SceneRenderer
	{
	public:
		static void Init();

		static void SetViewportSize(uint32_t width, uint32_t height);

		static void BeginScene(const Scene* scene);
		static void EndScene();

		static void SubmitEntity(Entity* entity);

		static std::pair<Ref<TextureCube>, Ref<TextureCube>> CreateEnviromentMap(const std::string& filepath);

		static Ref<Texture2D> GetFinalColorBuffer();

		// Temp
		static uint32_t GetFinalColorBufferRendererID();
	private:
		static void FlushDrawList();
		static void GeometryPass();
		static void CompositePass();
	};
}