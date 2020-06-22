#pragma once
#include "Moza/Renderer/Shader.h"

#include "Platform/OpenGL/OpenGLShaderUniform.h"

#include <string>
#include <glm/glm.hpp>

// TODO: REMOVE!
typedef unsigned int GLenum;

namespace Moza
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader() = default;
		OpenGLShader(const std::string& filePath);
		static Ref<OpenGLShader> CreateFromString(const std::string& source);
		~OpenGLShader();

		virtual void Reload() override;
		virtual void AddShaderReloadedCallback(const ShaderReloadedCallback& callback) override;

		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetVSMaterialUniformBuffer(Buffer buffer) override;
		virtual void SetPSMaterialUniformBuffer(Buffer buffer) override;

		/////////////////////////////////////////////////////////////////////////////
		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void SetFloat(const std::string& name, const float value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;
		/////////////////////////////////////////////////////////////////////////////

		virtual const std::string& GetName() const override { return m_Name; };
	private:
		void Load(const std::string& source);

		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		
		void Parse();
		void ParseUniform(const std::string& statement, ShaderDomain domain);
		void ParseUniformStruct(const std::string& block, ShaderDomain domain);
		ShaderStruct* FindStruct(const std::string& name);

		void Compile();
		void ResolveUniforms();
		void ValidateUniforms();

		int32_t GetUniformLocation(const std::string& name) const;

		void ResolveAndSetUniforms(const Scope<OpenGLShaderUniformBufferDeclaration>& decl, Buffer buffer);
		void ResolveAndSetUniform(OpenGLShaderUniformDeclaration* uniform, Buffer buffer);
		void ResolveAndSetUniformArray(OpenGLShaderUniformDeclaration* uniform, Buffer buffer);
		void ResolveAndSetUniformField(const OpenGLShaderUniformDeclaration& field, unsigned char* data, int32_t offset);
		
		/////////////////////////////////////////////////////////////////////////////////////////
		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		void UploadUniformMat4Array(const std::string& name, const glm::mat4& matrix, uint32_t count);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

		void UploadUniformStruct(OpenGLShaderUniformDeclaration* uniform, unsigned char* buffer, uint32_t offset);
		/////////////////////////////////////////////////////////////////////////////////////////

		virtual const ShaderUniformBufferList& GetVSRendererUniforms() const override { return m_VSRendererUniformBuffers; }
		virtual const ShaderUniformBufferList& GetPSRendererUniforms() const override { return m_PSRendererUniformBuffers; }
		virtual bool HasVSMaterialUniformBuffer() const override { return (bool)m_VSMaterialUniformBuffer; }
		virtual bool HasPSMaterialUniformBuffer() const override { return (bool)m_PSMaterialUniformBuffer; }
		virtual const ShaderUniformBufferDeclaration& GetVSMaterialUniformBuffer() const override { return *m_VSMaterialUniformBuffer; }
		virtual const ShaderUniformBufferDeclaration& GetPSMaterialUniformBuffer() const override { return *m_PSMaterialUniformBuffer; }
		virtual const ShaderResourceList& GetResources() const override { return m_Resources; }
	private:
		uint32_t m_RendererID = 0;
		std::string m_Name, m_AssetPath;
		bool m_Loaded = false;
		bool m_IsCompute = false;

		std::unordered_map<GLenum, std::string> m_ShaderSource;

		std::vector<ShaderReloadedCallback> m_ShaderReloadedCallbacks;

		ShaderUniformBufferList m_VSRendererUniformBuffers;
		ShaderUniformBufferList m_PSRendererUniformBuffers;
		Scope<OpenGLShaderUniformBufferDeclaration> m_VSMaterialUniformBuffer;
		Scope<OpenGLShaderUniformBufferDeclaration> m_PSMaterialUniformBuffer;
		ShaderResourceList m_Resources;
		ShaderStructList m_Structs;
	};
}