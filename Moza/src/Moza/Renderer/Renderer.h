#pragma once

#include "Moza/Renderer/RendererCommand.h"
#include "Moza/Renderer/RenderPass.h"
#include "Moza/Renderer/OrthographicCamera.h"
#include "Moza/Renderer/Shader.h"

#include "Moza/Renderer/Mesh.h"

namespace Moza
{
	class ShaderLibrary;

	class Renderer
	{
	public:
		typedef void(*RenderCommandFn)(void*);

		static void Init();
		static void Shutdown();

		static const Scope<ShaderLibrary>& GetShaderLibrary();
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		//template<typename FuncT>
		//static void Submit(FuncT&& func)
		//{
		//	return s_Instance->m_CommandQueue.Allocate(fn, size);
		//	auto renderCmd = [](void* ptr) {
		//		auto pFunc = (FuncT*)ptr;
		//		(*pFunc)();
		//
		//		// NOTE: Instead of destroying we could try and enforce all items to be trivally destructible
		//		// however some items like uniforms which contain std::strings still exist for now
		//		// static_assert(std::is_trivially_destructible_v<FuncT>, "FuncT must be trivially destructible");
		//		pFunc->~FuncT();
		//	};
		//	auto storageBuffer = GetRenderCommandQueue().Allocate(renderCmd, sizeof(func));
		//	new (storageBuffer) FuncT(std::forward<FuncT>(func));
		//}

		static void WaitAndRender();

		// ~Actual~ Renderer here... TODO: remove confusion later
		static void BeginRenderPass(const Ref<RenderPass>& renderPass);
		static void EndRenderPass();

		static void SubmitQuad(const Ref<MaterialInstance>& material, const glm::mat4& transform = glm::mat4(1.0f));
		static void SubmitFullscreenQuad(const Ref<MaterialInstance>& material, bool depthTest = true);
		static void SubmitMesh(const Ref<Mesh>& mesh, const glm::mat4& transform, const Ref<MaterialInstance>& overrideMaterial = nullptr);

	private:
		// static RenderCommandQueue& GetRenderCommandQueue();
	};
}