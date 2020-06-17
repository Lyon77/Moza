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

		//static void* Submit(RenderCommandFn fn, unsigned int size)
		//{
		//	return s_Instance->m_CommandQueue.Allocate(fn, size);
		//}

		void WaitAndRender();

		// ~Actual~ Renderer here... TODO: remove confusion later
		static void BeginRenderPass(const Ref<RenderPass>& renderPass) { s_Instance->IBeginRenderPass(renderPass); }
		static void EndRenderPass() { s_Instance->IEndRenderPass(); }

		static void SubmitMesh(const Ref<Mesh>& mesh) { s_Instance->SubmitMeshI(mesh); }

	private:
		void IBeginRenderPass(const Ref<RenderPass>& renderPass);
		void IEndRenderPass();

		void SubmitMeshI(const Ref<Mesh>& mesh);

		static Renderer* s_Instance;

		Ref<RenderPass> m_ActiveRenderPass;
		//RenderCommandQueue m_CommandQueue;
		Scope<ShaderLibrary> m_ShaderLibrary;
	};
}