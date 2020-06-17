#pragma once

#include "Moza/Core/Buffer.h"

#include "Moza/Renderer/ShaderUniform.h"

#include <string>
#include <glm/glm.hpp>

namespace Moza
{
	class Shader
	{
	public:
		using ShaderReloadedCallback = std::function<void()>;

		virtual ~Shader() = default;

		virtual void Reload() = 0;
		virtual void AddShaderReloadedCallback(const ShaderReloadedCallback& callback) = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, const float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		virtual const std::string& GetName() const = 0;

		virtual void SetVSMaterialUniformBuffer(Buffer buffer) = 0;
		virtual void SetPSMaterialUniformBuffer(Buffer buffer) = 0;

		virtual const ShaderUniformBufferList& GetVSRendererUniforms() const = 0;
		virtual const ShaderUniformBufferList& GetPSRendererUniforms() const = 0;
		virtual const ShaderUniformBufferDeclaration& GetVSMaterialUniformBuffer() const = 0;
		virtual const ShaderUniformBufferDeclaration& GetPSMaterialUniformBuffer() const = 0;

		virtual const ShaderResourceList& GetResources() const = 0;

		static Ref<Shader> Create(const std::string& filePath);
		static Ref<Shader> CreateFromString(const std::string& source);

		// Temporary, before we have an asset manager
		static std::vector<Ref<Shader>> s_AllShaders;
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		void Load(const std::string& filePath);
		void Load(const std::string& name, std::string& filePath);

		Ref<Shader>& Get(const std::string& name);

		bool Exists(const std::string name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}