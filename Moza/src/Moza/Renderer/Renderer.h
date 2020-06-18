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

		static const Scope<ShaderLibrary>& GetShaderLibrary() { return Get().m_ShaderLibrary; }
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		inline static Renderer& Get() { return *s_Instance; }

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
		//	auto storageBuffer = s_Instance->m_CommandQueue.Allocate(renderCmd, sizeof(func));
		//	new (storageBuffer) FuncT(std::forward<FuncT>(func));
		//}


		void WaitAndRender();

		// ~Actual~ Renderer here... TODO: remove confusion later
		static void BeginRenderPass(const Ref<RenderPass>& renderPass) { s_Instance->IBeginRenderPass(renderPass); }
		static void EndRenderPass() { s_Instance->IEndRenderPass(); }

		static void SubmitMesh(const Ref<Mesh>& mesh, const glm::mat4& transform, const Ref<MaterialInstance>& overrideMaterial = nullptr) { s_Instance->SubmitMeshI(mesh, transform, overrideMaterial); }

	private:
		void IBeginRenderPass(const Ref<RenderPass>& renderPass);
		void IEndRenderPass();

		void SubmitMeshI(const Ref<Mesh>& mesh, const glm::mat4& transform, const Ref<MaterialInstance>& overrideMaterial);

		static Renderer* s_Instance;

		Ref<RenderPass> m_ActiveRenderPass;
		//RenderCommandQueue m_CommandQueue;
		Scope<ShaderLibrary> m_ShaderLibrary;
	};
}